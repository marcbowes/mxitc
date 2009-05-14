/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "mxitc.h"


namespace MXit
{

namespace GUI

{

/****************************************************************************
**
** Author: Richard Baxter
**
** Dialog constructor
**
** ==== Arguments
** - app: owned by main.cpp
** - client: owned by main.cpp
**
****************************************************************************/
MXitC::MXitC(QApplication *app, MXit::Client *client) : QMainWindow ( 0 ), currentState(LOGGED_OUT), currentChatSession(NULL)
{
  
  setupUi(this);      /* from ui_dialog.h: generated from dialog.ui */
  if (QSystemTrayIcon::isSystemTrayAvailable()) {
    trayIcon = new QSystemTrayIcon(this);
  } else {
    trayIcon = NULL;
  }
  mxit = client;      /* store a copy */
  application = app;  /* store a copy */
  
  login = NULL;
   
  /* Loading settings */
  settings = new QSettings ( "mxitc", "env", this );
  
  /*------------------------------------------------------------------------------------------*/
  /* Adding MXitDockWidgets - appendDockWidget will restore their closed& floating states as well as add thme to the necessary data structures*/
  /*------------------------------------------------------------------------------------------*/
  #ifdef VARIABLE_DEBUGGER
  DockWidget::Debug * debugWidget = new DockWidget::Debug (this, theme);
  appendDockWidget(debugWidget,    Qt::RightDockWidgetArea, actionDebug_Variables);
  #endif
  
  optionsWidget = new DockWidget::Options (this, theme);
  appendDockWidget(optionsWidget,  Qt::RightDockWidgetArea, actionOptions);
  
  chatSessionsWidget = new DockWidget::ChatSessions (this, theme);
  appendDockWidget(chatSessionsWidget, Qt::LeftDockWidgetArea, actionChat_Sessions);
  
  contactsWidget = new DockWidget::Contacts (this, theme);
  appendDockWidget(contactsWidget, Qt::LeftDockWidgetArea, actionContacts);
  
  logWidget = new DockWidget::Log (this, theme);
  appendDockWidget(logWidget, Qt::RightDockWidgetArea, actionLogs);
  
  addContactWidget = new DockWidget::AddContact (this, theme);
  appendDockWidget(addContactWidget, Qt::LeftDockWidgetArea, actionAdd_Contact);
  
  /*------------------------------------------------------------------------------------------*/
  /*Settings restore */
  /*------------------------------------------------------------------------------------------*/
  /* TODO save window size and maximised state*/
  /* After all the MXitDockWidgets have been added, their attributes can be restored*/
  restoreState(settings->value("gui layout").toByteArray());
  
  /* Restoring theme information */
  optionsWidget->setBaseThemeDirectory(settings->value("themeBaseDirectory").toString());
  optionsWidget->setSelectedTheme(settings->value("selectedTheme").toString());
  
  
  resize(settings->value("mainWindowSize").toSize());
  
  /* normally the optionsWidget->setSelectedTheme will trigger the optionWidget's themeChanged SIGNAL which will (in a few lines) be connected to this class's themeChange SLOT
  We can't connect the themeChanged SIGNAL/SLOTs up since that would cause a QSettings save on the 'selected theme' on index 0 of the list and this->themeChanged which will overwrite the restored (correct!) QSettings value for 'selected theme' (something [TODO find out again] sets index to 0 => changes index of list => optionsWidget's loadTheme => this class's themeChanged => which overwrites the settings)*/
  themeChanged(); /* so we just call this manually since we know now the correct theme is selected*/
  
  /*------------------------------------------------------------------------------------------*/
  /* Connecting of functionality from child widgets to client*/
  /*------------------------------------------------------------------------------------------*/
  /* connecting the addContact functionality from the widget to the client*/
  connect(addContactWidget, 
          SIGNAL(addContact(const QString &, const QString &, const QString &, Protocol::Enumerables::Contact::Type, const QString &)), 
          mxit, 
          SLOT  (addContact(const QString &, const QString &, const QString &, Protocol::Enumerables::Contact::Type, const QString &))  );
  
  
  /*------------------------------------------------------------------------------------------*/
  /* Connecting contextMenu requests from child widgets */
  /*------------------------------------------------------------------------------------------*/
  connect(  chatSessionsWidget, 
            SIGNAL(contextMenuRequest(const QPoint &, const QString &)), 
            this , 
            SLOT(chatSessionsMenu(const QPoint &, const QString &))  );
          
  connect(  contactsWidget, 
            SIGNAL(contextMenuRequest(const QPoint &, const QString &)), 
            this , 
            SLOT(contactsMenu(const QPoint &, const QString &))  );
  
   connect( contactsWidget, SIGNAL(chatRequest ( QListWidgetItem * )),
            this, SLOT(chatRequestedViaContact ( QListWidgetItem * ) )  );
  
  /*------------------------------------------------------------------------------------------*/
  /* Connecting new variables SIGNAL from the client to tell debugWidget to update
  /*------------------------------------------------------------------------------------------*/
   #ifdef VARIABLE_DEBUGGER
  connect(mxit, SIGNAL(outgoingVariables(const VariableHash&)), debugWidget, SLOT(incomingVariableHash(const VariableHash&)));
  #endif
  
  /*TODO put this somewhere useful*/
  mainTextArea->setFocusProxy(chatInput);
  
  /*------------------------------------------------------------------------------------------*/
  /* Unsorted connects
  /*------------------------------------------------------------------------------------------*/
  
  /*------------------------------------------------------------------------------------------*/
  /* Connecting new variables SIGNAL from the widgets to the client
  /*------------------------------------------------------------------------------------------*/
  /*TODO sort out what connects need to go where*/
  
  connect(actionLogon_to_MXIT, SIGNAL(triggered()), this, SLOT(openLoginDialog()));
  connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
  
  connect(chatInput,  SIGNAL(returnPressed ()), this, SLOT(sendMessageFromChatInput()));
  

  connect(  mxit, SIGNAL(outgoingError(int, const QString &)), 
            this, SLOT(incomingError(int, const QString &)));
            
  connect(  mxit, SIGNAL(outgoingAction(Action)), 
            this, SLOT(incomingAction(Action)));
            
  connect(  mxit, SIGNAL(outgoingConnectionError(const QString &)), 
            this, SLOT(incomingConnectionError(const QString &)));
  
  
  connect(  chatSessionsWidget, SIGNAL(outgoingItemPressed ( QListWidgetItem *  )), 
            this, SLOT(setCurrentChatSession( QListWidgetItem *  )));
  
  connect(  optionsWidget, SIGNAL(gatewaySelected(bool)), this, SLOT(sendGateway( bool )));  

  connect(  optionsWidget, SIGNAL(themeChanged()), this, SLOT(themeChanged()));
  
  
  /*------------------------------------------------------------------------------------------*/
  /* Setting up status bar
  /*------------------------------------------------------------------------------------------*/
  statusLabel = new QLabel("No status set!");
  statusbar->addPermanentWidget(statusLabel);
  setStatusBar();
  
  /*------------------------------------------------------------------------------------------*/
  /* Loading client hash variables from QSettings and passing to client
  /*------------------------------------------------------------------------------------------*/
  
  /* this is the list of variables that should be loaded*/
  StringVec variables;
  variables.append("err");                  /* 0 = success, else failed */
  variables.append("url");                  /* URL that should be used for the Get PID request */
  variables.append("sessionid");            /* unique identifier to identify the session for code image answer */
  variables.append("captcha");              /* base64 encoded image data */
  variables.append("countries");            /* list of available countries (countrycode|countryname)
                                        * the list of country names should be presented to the user in order to
                                        * find the country code that should be used later on */
  variables.append("languages");            /* list of supported languages (locale|languagename)
                                        * the list of language names should be presented to the user and the
                                        * corresponding locale saved by the client to be used later on */
  variables.append("defaultCountryName");   /* country name of the country detenced from the requestors IP */
  variables.append("defaultCountryCode");   /* country code associated with the defaultCountryName */
  variables.append("regions");              /* a '|' seperated list of regions if requested */
  variables.append("defaultDialingCode");   /* dialing code associated with the defaultCountryName */
  variables.append("defaultRegion");        /* a region of the detected IP */
  variables.append("defaultNPF");           /* the national dialing prefix for the defaultCountryName, e.g. 0 */
  variables.append("defaultIPF");           /* the international dialing prefix for the defaultCountryName, e.g. 00 */
  variables.append("cities");               /* NOT IMPLEMENTED YET */
  variables.append("defaultCity");          /* the city of the detected IP */

  variables.append("loginname");
  variables.append("encryptedpassword");
  variables.append("dc");
  variables.append("soc1");
  variables.append("http1");
  variables.append("soc2");
  variables.append("http2");
  
  variables.append("locale");
  
  
  /* creating the variable hash*/
  VariableHash variableHash;
  
  bool hasAllVariables = true;
  
  Q_FOREACH(const QString &var, variables) {
    /*if a variable that is needed (i.e. is in variables) is not in settings then the auto login won't work and we need to bail out of the autologin process*/
    if (!settings->contains(var)) {
      hasAllVariables = false;
      break;
    }
    variableHash[var] = settings->value(var).toByteArray();
  }
  
    /*TODO sort out autologin, what if the user has deselected it in options, how do the hash variables get to the client, does the user still want an autologin but just promted or does the user want to retype in their password and captch each time?*/
    
  /*if the settings was able to load all the necessary variables, the autologin can commence*/
  /*TODO don't autologin if user has set it to not do so in options*/
  if (hasAllVariables) {
    loggingIn();
    mxit->authenticate(variableHash);
  }
  else {
    mxit->initialize();
    openLoginDialog();
  }
}


/****************************************************************************
**
** Author: Richard Baxter
**
** Dialog deconstructor
**
****************************************************************************/
MXitC::~MXitC()
{
  settings->sync(); /*just in case*/
  
  delete settings;
  statusbar->removeWidget(statusLabel);
  delete statusLabel;
  
  /* clean up MXitDockWidgets*/
  Q_FOREACH(const QDockWidget * dw, dockWidgets) {
    delete dw;
  }
  
  if (trayIcon)
    delete trayIcon;
}


/****************************************************************************
**
** Author: Richard Baxter
**
** the chatSessionsWidget requested a context menu
** pos is relative to the chatSession dock widget
**
** NOTE: it might seem like a good idea to generalise the context menu functions, it's not, i tried - rax
**
****************************************************************************/

/* trust me on this one, it's undefed after all the context menu stuffs are done - rax*/
/* FIXME overkill on the #defs? - rax*/
#define MENU_START(title) QMenu contextMenu(title, this); QHash<QString, QAction*> hash;
#define MENU_ITEM(y) hash[y] = new QAction (y, this); contextMenu.addAction(hash[y]);
#define MENU_EXEC(s) \
{ \
QAction* a = contextMenu.exec( pos ); \
bool __ret = true; \
if (a){ s = a->text(); __ret = false;} \
Q_FOREACH(QAction * act, hash){ delete act; } \
if (__ret) return;  \
}
  
void MXitC::chatSessionsMenu(const QPoint & pos, const QString& chatSessionName) {
  
  //ChatSession & chatSession = chatSessions[chatSessionName];
  
  MENU_START(chatSessionName);
  
  MENU_ITEM("Close Chat");
 
  QString selection;
  MENU_EXEC(selection);
  
  if (selection == "Close Chat") {
    /* closes chat */
    /*TODO logging of chats!*/
    
    closeChatSession(chatSessionName);
    
  }
}


/****************************************************************************
**
** Author: Richard Baxter
**
** the contactsWidget requested a context menu
** pos is relative to the contacts dock widget
**
****************************************************************************/

void MXitC::contactsMenu(const QPoint & pos, const QString& nickname) {
  
  MXit::Contact & contact = contacts[nicknameToContactAddress[nickname]];
  
  
  MENU_START(nickname);
  
  if (contact.presence == Protocol::Enumerables::Contact::Unaffiliated) {
    MENU_ITEM("Accept");
    MENU_ITEM("Reject");
    MENU_ITEM("Reject Permanently");
  }
  else {
    MENU_ITEM("Chat");
    MENU_ITEM("Change Nickname");
    MENU_ITEM("Change Group");
    MENU_ITEM("Send File");
    MENU_ITEM("Remove Contact");
  }
 
  QString selection;
  MENU_EXEC(selection);
  
  
  //qDebug() << selection;
  
  if (contact.presence == Protocol::Enumerables::Contact::Unaffiliated) {
    if (selection == "Accept") 
    {
      /* TODO ask user to select groupname and nickname - duplicate nicknames must be implemented!*/
      QSet<QString> groups = getGroupSet();
      Dialog::AllowSubscription allow (contact.inviteMessage, contact.nickname, groups, this);
      
      /*TODO make sure user doesn't add a nickname that already exists - related TODO about duplicate names*/
      if (allow.exec() == QDialog::Accepted) {
        
        mxit->allowSubscription(contact.contactAddress, allow.getGroup(), allow.getNickname());
        logWidget->logMessage("GUI:: "+contact.nickname+" subscribed to");
      }
        
    }
    else if (selection == "Reject") 
    {
      QMessageBox sure;
      sure.setText("Are you sure you wish to reject \""+contact.nickname+"\"");
      if (contact.inviteMessage != "")
        sure.setInformativeText(nickname+" sent you an invite message: \"" + contact.inviteMessage+"\"");
      sure.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
      sure.setDefaultButton(QMessageBox::Cancel);
      
      if (sure.exec() == QMessageBox::Ok) {
        /* send rejection to client*/
        logWidget->logMessage("GUI:: "+contact.nickname+" subscription denyed");
        mxit->denySubscription(contact.contactAddress);
        removeContactFromGUI(contact.contactAddress);
      }
    }
    else if (selection == "Reject Permanently") 
    {
      QMessageBox sure;
      sure.setText("Are you sure you wish to reject permanently \""+contact.nickname+"\"");
      if (contact.inviteMessage != "")
        sure.setInformativeText("In order to unblock, you need to add "+nickname+" as a contact\n"+nickname+" sent you an invite message: \"" + contact.inviteMessage+"\"");
      sure.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
      sure.setDefaultButton(QMessageBox::Cancel);
      
      if (sure.exec() == QMessageBox::Ok) {
        /* send permanent rejection to client*/
        logWidget->logMessage("GUI:: "+contact.nickname+" subscription permanently denyed");
        mxit->denySubscription(contact.contactAddress, true);
        removeContactFromGUI(contact.contactAddress);
      }
    }
  }
  else 
  {
    if (selection == "Chat") 
    {
      chatRequestedViaContact(nickname);
    }
    else if (selection == "Change Nickname") {
      /* TODO */
    }
    else if (selection == "Change Group") {
      /* TODO */
    }
    else if (selection == "Send File") {
      /* TODO */
    }
    else if (selection == "Remove Contact") {
      
      QMessageBox sure;
      sure.setText("Are you sure you wish to remove \""+nickname+"\"");
      sure.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
      sure.setDefaultButton(QMessageBox::Cancel);
      
      if (sure.exec() == QMessageBox::Ok) {
        logWidget->logMessage("GUI:: contact \""+contact.contactAddress+"\" removed");
        mxit->removeContact(contact.contactAddress);
        removeContactFromGUI(contact.contactAddress);
      }
    }
  }
}

#undef MENU_START
#undef MENU_ITEM
#undef MENU_EXEC


/****************************************************************************
**
** Author: Richard Baxter
**
** Removes the contact from the GUI and any associated chat sessions
** TODO what if the contact is in a group chat and you remove them?
**
****************************************************************************/

void MXitC::removeContactFromGUI(const QString& contactAddress) {

  /*remove any accociated chat sessions */
  /*TODO what if the contact is in a group chat and you remove them? */

  QString nickname = contacts[contactAddress].nickname;
  if (chatSessions.contains(nickname))
    closeChatSession(nickname);

  nicknameToContactAddress.remove(nickname);
  contacts.remove(contactAddress);
  
  refreshContacts();
  refreshChatSessions();
}
  
/****************************************************************************
**
** Author: Richard Baxter
**
** Starts a chat session with a certain contact if one has not been done so already. If one has already been created then this function will simply set the current
** this function does not raise the chatSessions window to the front, that should be handled higher up
**
****************************************************************************/

void MXitC::startChatSessionWithContact (MXit::Contact & contact) {
  
  if (!chatSessions.contains(contact.nickname)) {
    /* create new ChatSession */
    chatSessions[contact.nickname] = ChatSession(&contact);    
  }
  /* whether the chat session was created in this function or not, we must select it as the current chat session*/
  setCurrentChatSession (contact.nickname); 
}
  
/****************************************************************************
**
** Author: Richard Baxter
**
** Closes and cleans up the requested chat session
** NOTE assumes the chatSessionName is correct, error checking should be done higher up!
** FIXME all logs will be lost if you do this, logging system still to be implemented
**
****************************************************************************/

void MXitC::closeChatSession(const QString & chatSessionName) {

  /* if user closes the currentChatSession*/
  if(chatSessionName == currentChatSession->chatSessionName)
     currentChatSession = 0;
     
  chatSessions.remove(chatSessionName);
  refreshChatSessions();
  refreshChatBox();
}



/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

QSet<QString> MXitC::getGroupSet() {
  
  QSet<QString> ret;
 
  Q_FOREACH(const MXit::Contact & c, contacts) {
    if (!ret.contains(c.group))
      ret.insert(c.group);
  } 
  
  return ret;
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Requests the new gateway that should be used to the client 
**
****************************************************************************/

/* TODO fix up the gateway stuff and the gateway stuff in options*/
void MXitC::sendGateway(bool http)
{
  if (http)
    mxit->setGateway(mxit->variableValue("http1"));
  else
    mxit->setGateway(mxit->variableValue("soc1"));
}

/****************************************************************************
**
** Author: Richard Baxter
**
** This function:
**  appends a MXitDockWidget to a specified area
**  connectes it to a specified action that will toggle it's visibility and raise it to the front when triggered
**  adds it to the vector of dockwidgets in the mainwindow so that they can be itterated over
**  loads the various attributes from settings that QMainWindow::saveState() doesn't save 
**  hooks it up to the saveLayout function when it moves or changes state so that the layout is saved as the user changes it
**
****************************************************************************/

void MXitC::appendDockWidget(MXitDockWidget * dockWidget, Qt::DockWidgetArea area, QAction* action){

  /* adding to vector */
  dockWidgets.append(dockWidget);
  
  /* setting up features and adding it to this class (QMainWindow)*/
  dockWidget->setFeatures (QDockWidget::AllDockWidgetFeatures);
  addDockWidget(Qt::RightDockWidgetArea, dockWidget);
  dockWidget->setVisible ( false );
    
  /* loading visibility and floating attributes*/
  dockWidget->setVisible(settings->value(QString("visible?")+dockWidget->objectName ()).toBool());
  dockWidget->setFloating(settings->value(QString("floating?")+dockWidget->objectName ()).toBool());
    
  /* connecting the associated action to it's toggleVisibility SLOT*/
  /* TODO still haven't figured out how to check if the widget is raised or not, right now it only toggles visibility and doesn't cycle from visible but not raised -> visible and raised -> not visible -> visible and raised*/
  connect(action, SIGNAL(triggered()), dockWidget, SLOT(toggleVisibility()));
  
  /* hooking it up so that if anything cahnges, the settings will pick it up and save it's location*/
  connect(
          dockWidget, SIGNAL(dockLocationChanged (Qt::DockWidgetArea)), 
          this, SLOT(saveLayout(Qt::DockWidgetArea)));
  connect(
          dockWidget, SIGNAL(visibilityChanged ( bool ) ), 
          this, SLOT(saveLayout( bool )));
  
}


/****************************************************************************
**
** Author: Richard Baxter
**
** saves all the widget's attributes
**
****************************************************************************/


/* this was provided so that it could be connected nicely*/
void MXitC::saveLayout(bool b) {
}

/*Qt::DockWidgetArea is never actually used, it's there so this function could be connected nicely*/
void MXitC::saveLayout(Qt::DockWidgetArea area) {
  
  Q_FOREACH(const QDockWidget * dw, dockWidgets) {
  
    settings->setValue(QString("visible?")+dw->objectName (), dw->isVisible());
    settings->setValue(QString("floating?")+dw->objectName (), dw->isFloating());
  }
  
  settings->setValue("gui layout", saveState());
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void MXitC::resizeEvent ( QResizeEvent * event ) {

  
  settings->setValue("mainWindowSize", this->size ());

}

/****************************************************************************
**
** Author: Richard Baxter
**
** When the clien receives a packet, it usually means that the GUI must be notified of some action
** this function receives the actions form the client and reacts accordingly
**
****************************************************************************/

void MXitC::incomingAction(Action action)
{
  switch(action) {
    //--------------------------------------
    case LOGGED_IN:
      
      if (currentState == LOGGED_IN)
        ;/* do nothing TODO */
      else /* if (currentState == LOGGED_OUT) */
      {
        /* TODO get the PID and encrypted password*/
        
        /* vector of variables that'll be saved in settings */
        StringVec variables;
        variables.append("err");                  /* 0 = success, else failed */
        variables.append("url");                  /* URL that should be used for the Get PID request */
        variables.append("sessionid");            /* unique identifier to identify the session for code image answer */
        variables.append("captcha");              /* base64 encoded image data */
        variables.append("countries");            /* list of available countries (countrycode|countryname)
                                             * the list of country names should be presented to the user in order to
                                             * find the country code that should be used later on */
        variables.append("languages");            /* list of supported languages (locale|languagename)
                                             * the list of language names should be presented to the user and the
                                             * corresponding locale saved by the client to be used later on */
        variables.append("defaultCountryName");   /* country name of the country detenced from the requestors IP */
        variables.append("defaultCountryCode");   /* country code associated with the defaultCountryName */
        variables.append("regions");              /* a '|' seperated list of regions if requested */
        variables.append("defaultDialingCode");   /* dialing code associated with the defaultCountryName */
        variables.append("defaultRegion");        /* a region of the detected IP */
        variables.append("defaultNPF");           /* the national dialing prefix for the defaultCountryName, e.g. 0 */
        variables.append("defaultIPF");           /* the international dialing prefix for the defaultCountryName, e.g. 00 */
        variables.append("cities");               /* NOT IMPLEMENTED YET */
        variables.append("defaultCity");          /* the city of the detected IP */
  
        variables.append("loginname");
        variables.append("encryptedpassword");
        variables.append("dc");
        variables.append("soc1");
        variables.append("http1");
        variables.append("soc2");
        variables.append("http2");
        
        /* saveing variables to settings */
        
        Q_FOREACH(const QString &var, variables) {
          settings->setValue(var, mxit->variableValue(var));
        }
        settings->sync();
        
        currentState = LOGGED_IN;
        setStatusBar();
        
        /* closing the login window if it is open (i.e. login != NULL)*/
        if (login != NULL) {
          login->close();
        }
      }
      
      break;
      
    //--------------------------------------
    case LOGGED_OUT:
      
      if (currentState == LOGGED_OUT)
        ;/* do nothing TODO */
      else /* if (currentState == LOGGED_IN) */
      {
        currentState = LOGGED_OUT;
        setStatusBar();
      }
      break;
      
    //--------------------------------------
    case CONTACTS_RECEIVED:
      logWidget->logMessage("GUI::CONTACTS_RECEIVED");
      contactsReceived();
      break;
      
    //--------------------------------------
    case MESSAGE_RECEIVED:
      logWidget->logMessage("GUI::MESSAGE_RECEIVED");
      messageReceived();
    break;
    
    //--------------------------------------
    case SUBSCRIPTIONS_RECEIVED:
      logWidget->logMessage("GUI::SUBSCRIPTIONS_RECEIVED");
      subscriptionsReceived();
    break;
  }

}


/****************************************************************************
**
** Author: Richard Baxter
**
** deals with incoming subscriptions (not-yet-accepted contacts)
**
****************************************************************************/
void MXitC::subscriptionsReceived(){

  /* packet type 51. Get new subscriptions 
  
      contactAddress0 \1 nickname0 \1 type0 \1 hiddenLoginname0 \1 msg0 \1 groupchatmod0\0
      ... \0
      contactAddressN \1 nicknameN \1 typeN \1 hiddenLoginnameN \1 msgN \1 groupchatmodN
  */
  //qDebug() << "subscriptionsReceived";
  /* loop over all contacts*/
  Q_FOREACH(const QByteArray& contact, mxit->variableValue("contacts").split('\0')) 
  {
    if (contact == "") break; // the last split is always ""
    
    /* breaking up into fields:
      contactAddressN \1 nicknameN \1 typeN \1 hiddenLoginnameN \1 msgN \1 groupchatmodN
    */ 
    QVector<QByteArray> fields = QVector<QByteArray>::fromList ( contact.split('\1') );
    
    //qDebug() << fields;
    
    QString contactAddress = fields[0];
    
    ensureExistanceOfContact(contactAddress);
    MXit::Contact &c = contacts[contactAddress];
    
    {
      using namespace Protocol::Enumerables::Contact;
      c.contactAddress = fields[0];
      c.nickname = fields[1];
      c.type = (Type)fields[2].toUInt();
      c.presence = Unaffiliated;
      //c.hidden = fields[3].toBool();
      c.inviteMessage = fields[4]; 
      // TODO what do we do with hidden status?
    }
    
    nicknameToContactAddress[c.nickname] = c.contactAddress;
    /* TODO This whole creating acontact thing appears elsewhere in code, absract it? - rax*/
    
    /*FIXME deal with duplicate names!! what if a potential contact has the same name as one of your other contacts or other subsriptions*/
    
    refreshContacts(); /* the contacts list must be refreshed to reflect the new potential contact*/
  }
  
}


/****************************************************************************
**
** Author: Richard Baxter
**
** deals with incoming contacts
**
****************************************************************************/
void MXitC::contactsReceived(){

  /* fetch contacts */
  /*  
  /* packet type 3. Get contacts 
      group0 \1 contactAddress0 \1 nickname0 \1 presence0 \1 type0 \1 mood \0
      ...
      groupN \1 contactAddressN \1 nicknameN \1 presenceN \1 typeN \1 mood
  */
  
  //QSet<QString> shouldBeInHash;
  
  //qDebug() << "contactsReceived";
  /* loop over all contacts*/
  Q_FOREACH(const QByteArray& contact, mxit->variableValue("contacts").split('\0')) 
  {
    if (contact == "") break; // the last split is always ""
    
    /* breaking up into fields:
      group0 \1 contactAddress0 \1 nickname0 \1 presence0 \1 type0 \1 mood
    */
    QVector<QByteArray> fields = QVector<QByteArray>::fromList ( contact.split('\1') );
    
    //qDebug() << fields;
    
    QString contactAddress = fields [1];
    
    bool newContact = ensureExistanceOfContact(contactAddress);
    MXit::Contact &c = contacts[contactAddress];
    
    {
      using namespace Protocol::Enumerables::Contact;
      c.group           = fields[0];
      c.contactAddress  = fields[1];
      c.nickname        = fields[2];
      c.presence        = (Presence)fields[3].toUInt();
      c.type            = (Type)fields[4].toUInt();
      c.mood            = (Mood)fields[5].toUInt();
    }
    
    //shouldBeInHash.insert(c.contactAddress);
    
    /* TODO should this if be here? investigate - see subscriptions received for more TODO's related to this*/
    //if(newContact) {
      nicknameToContactAddress[c.nickname] = c.contactAddress;
    //}
  }
  
  /* NOTE keep this code here in case I can still use it*/
  //Q_FOREACH(const MXit::Contact & c, contacts.values()) {
  //  if (!shouldBeInHash.contains(c.contactAddress)) {
  //   /* then the contact in the hash is not supposed to be there, remove*/ 
  //    nicknameToContactAddress.remove(c.nickname);
  //    contacts.remove(c.contactAddress);
  //  }
  //}
  
  refreshContacts();
        
}

/****************************************************************************
**
** Author: Richard Baxter
**
** ensures that the contact exists in the contacts hash, if it is new this function return true
** (if it already exists the function return false)
**
****************************************************************************/

bool MXitC::ensureExistanceOfContact(const QString & contactAddress) {
  if (!contacts.contains(contactAddress)) {
    contacts[contactAddress] = MXit::Contact();
    return true;
  }
  return false;
  
}
/****************************************************************************
**
** Author: Richard Baxter
**
** ensures that the chatSessions exists in the chatSessions hash, if it is new this function return true
** (if it already exists the function return false)
**
****************************************************************************/

bool MXitC::ensureExistanceOfChatSession(MXit::Contact & contact) {
  if (!chatSessions.contains(contact.nickname)) {
    chatSessions[contact.nickname] = MXit::ChatSession(&contact);
    return true;
  }
  return false;
  
}

/****************************************************************************
**
** Author: Richard Baxter
**
** reads in a new received message and updates the associated chat session (or creates a new one if necessary)
**
****************************************************************************/

void MXitC::messageReceived(){

  /*FIXME only handles single user stuff atm! group chat to follow*/
  QString contactAddress = mxit->variableValue("contactAddress");
  //qDebug() << contactAddress;
  if (contacts.contains(contactAddress)) { /*since we are assuming the chat is from a single contact not a group chat*/
  
    MXit::Contact& sender = contacts[contactAddress];
    //qDebug() << sender.nickname;
    ensureExistanceOfChatSession(sender);
    chatSessions[sender.nickname].incomingMessage( Message(sender, mxit->variableValue("message")) );
    
    /* if the chatSession that received the message is the one displayed, we need to set the unread message variable to false so that it won't be marked as 'unread'*/
    if (currentChatSession)
      currentChatSession->unreadMessage = false;
    
    
    //Q_FOREACH(const ChatSession & c, chatSessions.values()) {
      //qDebug() << c.chatSessionName;
    //}
    refreshChatSessions(); /* show unread messages, new chat sessions etc*/
    refreshChatBox();
  }
  else {
    qDebug() << "wtf unknown contact!"; 
    /*TODO handle this eror case, even though it should NEVER happen - rax*/
  }
        
}


/****************************************************************************
**
** Author: Richard Baxter
**
** refreshes all the widgets that need refreshing (for the new theme icons) and saves what theme is selected to settings
**
****************************************************************************/

void MXitC::themeChanged(){
  setWindowIcon(theme.windowIcon);
  if (trayIcon) {
    trayIcon->setIcon(theme.windowIcon);
    if (!trayIcon->isVisible()) /* skip warning about no icon before a theme is set */
      trayIcon->show();
  }
  refreshChatSessions();
  refreshContacts();
  
  addContactWidget->refresh(); /* since it contains icons*/
  /*TODO maybe make refresh a MXitDockWidget function and loop over all widgets. i.e. generalise? - rax*/
  
  chatSessionsWidget->setStyleSheet(theme.contact.stylesheet);
  
  /* saving theme to settings*/
  settings->setValue("themeBaseDirectory", optionsWidget->getBaseThemeDirectory());
  settings->setValue("selectedTheme", optionsWidget->getSelectedTheme());
  
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Sets the curent contact the user is chatting to
** NOTE: Assumes that the chatSessionName is correct! error checking should be done higher up
**
****************************************************************************/

/* provided so that the ListWidget can hook up to it*/
void MXitC::setCurrentChatSession(QListWidgetItem * item){
  setCurrentChatSession(item->text());
}

/****************************************************************************/

void MXitC::setCurrentChatSession(const QString & chatSessionName){

  if (currentChatSession)
    currentChatSession->chatInputText = chatInput->text();
  
  currentChatSession = &chatSessions[chatSessionName];
  currentChatSession->unreadMessage = false;
  
  refreshChatBox();
  chatSessionsWidget->refresh(chatSessions.values());
  
  chatInput->setText(currentChatSession->chatInputText);
  
}

/****************************************************************************
**
** Author: Richard Baxter
**
** chat with specified user was requested 
**
****************************************************************************/

void MXitC::chatRequestedViaContact ( QListWidgetItem * item) {
  chatRequestedViaContact(item->text());
}

void MXitC::chatRequestedViaContact ( const QString& nickname ) {

  MXit::Contact & contact = contacts[nicknameToContactAddress[nickname]];
  startChatSessionWithContact(contact);
  
  /* since the user requested a chat, bring forward the chatSessions dockwindow*/
  chatSessionsWidget->raise();
  
  chatSessionsWidget->selectItem(chatSessions[contact.nickname].chatSessionName);
  
  chatInput->setFocus(Qt::OtherFocusReason);
}



/****************************************************************************
**
** Author: Richard Baxter
**
** Refreshes the chatBox area TODO change name to refreshMainTextArea
**
****************************************************************************/

void MXitC::refreshChatBox(){

  if (currentChatSession)
    chattingToLabel->setText(currentChatSession->chatSessionName);
  else
    chattingToLabel->setText("Chatting to nobody");
    
  mainTextArea->clear();
  //mainChatArea->setRowCount(0);
  if (currentChatSession != NULL) {
    Q_FOREACH(const Message *m, currentChatSession->chatHistory) {
    
      mainTextArea->append (  QString("<") +(m->contact?m->contact->nickname:QString("You")) + QString("> ") +m->message );
    }
  }
  
  
}


/****************************************************************************
**
** Author: Richard Baxter
**
** Refreshes the chatSessions widget
**
****************************************************************************/

void MXitC::refreshChatSessions(){
  chatSessionsWidget->refresh(chatSessions.values());
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Refreshes the chatSessions area
**
****************************************************************************/

void MXitC::refreshContacts(){
  contactsWidget->refresh(contacts.values());
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Sets the status bar text based on the state of the application
**
****************************************************************************/

void MXitC::setStatusBar()
{
  switch (currentState) {
  
    case LOGGED_IN:  statusLabel->setText("LOGGED_IN");  break;
    case LOGGED_OUT: statusLabel->setText("LOGGED_OUT"); break;
    case LOGGING_IN: statusLabel->setText("LOGGING_IN"); break;
  
        
  }
  logWidget->logMessage("GUI:: State set to "+statusLabel->text());
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Sends the chatInput to the ougoing slot and clears the chat lineText object
**
****************************************************************************/

void MXitC::sendMessageFromChatInput()
{
  outgoingMessage(chatInput->text()); /*signals to this classes outgoing messages so it can go to the client*/
  chatInput->setText("");
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Handles an incoming connection error (recieved from the client)
**
****************************************************************************/
void MXitC::incomingConnectionError(const QString & errorString)
{
  logWidget->logMessage(QString("GUI:: Error %1").arg(errorString));
  
  if (trayIcon && trayIcon->isVisible()) {
    trayIcon->showMessage("Network Error",  errorString);
  }
  
  if (login != NULL) {
    login->resetButtons();
  }
}


/****************************************************************************
**
** Author: Richard Baxter
**
** Handles an incoming error (recieved from the client)
**
****************************************************************************/

void MXitC::incomingError(int errorCode, const QString & errorString)
{
  logWidget->logMessage("GUI:: Error "+QString("(%1) %2").arg(errorCode).arg(errorString));
  
  if (trayIcon && trayIcon->isVisible()) {
    trayIcon->showMessage(QString("Error #%1").arg(errorCode),  errorString);
  }
  
  if (login != NULL) {
    login->resetButtons();
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Handles an outgoing message (sends it to the network controller)
**
****************************************************************************/

void MXitC::outgoingMessage(const QString & message)
{
  if (currentChatSession) {
    currentChatSession->incomingMessage( Message ( message) );
    currentChatSession->unreadMessage = false;
    mxit->sendMessage(currentChatSession->mainContact->contactAddress, message, Protocol::Enumerables::Message::Normal, 0);
    refreshChatBox();
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
** The close event so that it displays the close dialog before closeing
**
****************************************************************************/

void MXitC::closeEvent(QCloseEvent *event)
{
  QMessageBox::StandardButton answer = QMessageBox::question ( 
                              this ,/*QWidget * parent*/
                              "Quit?", /* const QString & title */
                              "Are you sure you want to quit?", /* const QString & text */
                              QMessageBox::Ok|QMessageBox::Cancel /*StandardButtons buttons = Ok*/ 
                              /*StandardButton defaultButton = NoButton */);
                              
  if (answer == QMessageBox::Ok) 
    event->accept();
  else
    event->ignore();

}

/****************************************************************************
**
** Author: Richard Baxter
**
** tells the gui that it's state should be logging in
**
****************************************************************************/

void MXitC::loggingIn(){
  currentState = LOGGING_IN;
  setStatusBar();
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Opens the login dialog
**
****************************************************************************/

void MXitC::openLoginDialog(){
  
  
  login = new Dialog::Login(this, mxit, settings);
  connect(login, SIGNAL(loggingIn()), this, SLOT(loggingIn()));
  login->exec();
  disconnect(login, SIGNAL(loggingIn()), this, SLOT(loggingIn()));
  delete login;
  login = NULL;
}

  
  
}

}


/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

//#include <QWebView>
#include "mxitc.h"
#include <QTextDocument>
#include <QWebFrame>

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
MXitC::MXitC(QApplication *app, MXit::Client *client) : QMainWindow ( 0 ), currentState(LOGGED_OUT), currentConversation(NULL)
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
  
  conversations = new MXit::Conversations(&addressBook, QDir(settings->value("logConversations").toString()));
  
  /*------------------------------------------------------------------------------------------*/
  /* Adding MXitDockWidgets - appendDockWidget will restore their closed& floating states as well as add thme to the necessary data structures*/
  /*------------------------------------------------------------------------------------------*/
  
  logWidget = new DockWidget::Log (this, theme);
  appendDockWidget(logWidget, Qt::RightDockWidgetArea, actionLogs);
  
  DockWidget::Debug * debugWidget = new DockWidget::Debug (this, theme);
  appendDockWidget(debugWidget,    Qt::RightDockWidgetArea, actionDebug_Variables);
  
  optionsWidget = new DockWidget::Options (this, theme, *settings);
  appendDockWidget(optionsWidget,  Qt::RightDockWidgetArea, actionOptions);
  
  conversationsWidget = new DockWidget::Conversations (this, theme, *mxit, *conversations);
  appendDockWidget(conversationsWidget, Qt::LeftDockWidgetArea, actionConversations);
  
  contactsWidget = new DockWidget::Contacts (this, theme, *mxit, addressBook, *conversations);
  appendDockWidget(contactsWidget, Qt::LeftDockWidgetArea, actionContacts);
  
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
  
  if(settings->contains("mainWindowSize"))
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
  
  
  
  
  /*this should be somewhere else*/
  //connect( contactsWidget, SIGNAL(chatRequest ( QListWidgetItem * )),
  //          this, SLOT(chatRequestedViaContact ( QListWidgetItem * ) )  );
  
  /*------------------------------------------------------------------------------------------*/
  /* Connecting new variables SIGNAL from the client to tell debugWidget to update
  /*------------------------------------------------------------------------------------------*/
  connect(mxit, SIGNAL(outgoingVariables(const VariableHash&)), debugWidget, SLOT(incomingVariableHash(const VariableHash&)));
  
  /*TODO put this somewhere useful*/
  mainWebView->setFocusProxy(chatInput);
  
  /*------------------------------------------------------------------------------------------*/
  /* Unsorted connects
  /*------------------------------------------------------------------------------------------*/
  
  connect(mxit, SIGNAL(environmentReady()), this, SLOT(environmentVariablesReady()));
  
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
            this, SLOT(incomingAction(Action))  );
            
  connect(  mxit, SIGNAL(outgoingConnectionError(const QString &)), 
            this, SLOT(incomingConnectionError(const QString &))  );
  
  connect(  contactsWidget, SIGNAL(conversationRequest ( const Contact *  )), 
            this, SLOT(setCurrentConversation( const Contact *  )));
  
  connect(  conversationsWidget, SIGNAL(conversationRequest ( const Conversation *  )), 
            this, SLOT(setCurrentConversation( const Conversation *  )));
  
  connect(  optionsWidget, SIGNAL(gatewaySelected(const QString&)), 
            this, SLOT(sendGatewayToClient(const QString&))  );  

  
  /*------------------------------------------------------------------------------------------*/
  /* Hooking up themeing refreshes */
  /*------------------------------------------------------------------------------------------*/
  
  connect(  optionsWidget, SIGNAL(themeChanged()), this, SLOT(themeChanged()));
  connect(  optionsWidget, SIGNAL(themeChanged()), conversationsWidget, SLOT(refreshThemeing()));
  connect(  optionsWidget, SIGNAL(themeChanged()), contactsWidget, SLOT(refreshThemeing()));
  
  /* Conversation log dir */
  connect(optionsWidget, SIGNAL(conversationLogDirectorySelected(const QDir&)),
          this,         SLOT(logConversations(const QDir&)));
  
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
** function is called when the environment variables are set
**
****************************************************************************/

void MXitC::environmentVariablesReady() {

  /*TODO make a log*///qDebug() << "environmentVariablesReady";
  /* notify gateway */
  
  optionsWidget->addGateway(mxit->variableValue("soc1"));
  optionsWidget->addGateway(mxit->variableValue("soc2"));
  optionsWidget->addGateway(mxit->variableValue("http1"));
  optionsWidget->addGateway(mxit->variableValue("http2"));
  
  if (settings->contains("gateway")) {
    optionsWidget->setSelectedGateway(settings->value("gateway").toString());
  }
  else {
    optionsWidget->setSelectedGateway(mxit->variableValue("soc1"));
  }
}





  
/****************************************************************************
**
** Author: Richard Baxter
**
** Closes and cleans up the requested chat session
** NOTE assumes the conversationName is correct, error checking should be done higher up!
** FIXME all logs will be lost if you do this, logging system still to be implemented
**
****************************************************************************/

/* moved elsewhere*/
/*void MXitC::closeConversation(Conversation* conversation) {

  /* if user closes the currentConversation*/
  /*if(conversationName == currentConversation->conversationName)
     currentConversation = 0;
     
  conversations.remove(conversation);
  refreshConversations();
  refreshChatBox();
}*/


/****************************************************************************
**
** Author: Richard Baxter
**
** Requests the new gateway that should be used to the client 
**
****************************************************************************/

/* TODO fix up the gateway stuff and the gateway stuff in options*/
void MXitC::sendGatewayToClient(const QString& gateway)
{
  settings->setValue("gateway", gateway);
  mxit->setGateway(gateway);
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
          
  if (dockWidget != logWidget)
    connect(
          dockWidget, 
          SIGNAL(sendLog ( const QString& )), 
          logWidget, 
          SLOT(logMessage( const QString& ))  );
  
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
      
      logWidget->logMessage("GUI::LOGGED_IN");
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
      logWidget->logMessage("GUI::LOGGED_OUT");
      
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
      addressBook.addOrUpdateContacts(mxit->variableValue("contacts"));
      break;
      
    //--------------------------------------
    case MESSAGE_RECEIVED:
      logWidget->logMessage("GUI::MESSAGE_RECEIVED");
      messageReceived();
    break;
    
    //--------------------------------------
    case SUBSCRIPTIONS_RECEIVED:
      logWidget->logMessage("GUI::SUBSCRIPTIONS_RECEIVED");
      addressBook.addSubscriptions(mxit->variableValue("contacts"));
    break;
  }

}

/****************************************************************************
   __  ___                      _          
  /  |/  /__ ___ ___ ___ ____ _(_)__  ___ _
 / /|_/ / -_|_-<(_-</ _ `/ _ `/ / _ \/ _ `/
/_/  /_/\__/___/___/\_,_/\_, /_/_//_/\_, / 
                        /___/       /___/  

****************************************************************************/

/****************************************************************************
**
** Author: Richard Baxter
**
** reads in a new received message and updates the associated chat session (or creates a new one if necessary)
**
****************************************************************************/

void MXitC::messageReceived(){

  /* make sure conversation exists */
  ensureExistanceOfConversation(mxit->variableValue("contactAddress"));
  
  conversations->addMessage(  mxit->variableValue("contactAddress"),
                              mxit->variableValue("dateTime"), 
                              mxit->variableValue("time"), /*TODO check if this exists*/
                              mxit->variableValue("contactAddress"), 
                              mxit->variableValue("flags"),
                              mxit->variableValue("message"));
  /*TODO send message to conversations*/
  
  /*TODO hook up Conversations updated signal to the contactsWidget (do in contactDockWidget class!)*/
  refreshChatBox();
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
  if (currentConversation) {
    //currentConversation->incomingMessage( Message ( message) );
    //currentConversation->unreadMessage = false;
    
    /*TODO send message to Conversations*/
    
    Q_FOREACH(const Contact* contact, currentConversation->getContacts()) {
      mxit->sendMessage(contact->contactAddress, message, Protocol::Enumerables::Message::Normal /* FIXME? */, Protocol::Enumerables::Message::MayContainMarkup);
      
     /*void addMessage( const QByteArray &contactAddress,
                        const QByteArray &dateTime, 
                        const QByteArray &type,
                        const QByteArray &id, 
                        const QByteArray &flags,
                        const QByteArray &msg);*/
      
      conversations->addMessage(  QByteArray(), /*FIXME, should be 'me'*/
                                  mxit->variableValue("dateTime"),  /*FIXME, where do i get this from?*/
                                  mxit->variableValue("time"), /*FIXME, where do i get this from?*/
                                  QByteArray().append (contact->contactAddress),
                                  mxit->variableValue("flags"), /*FIXME, where do i get this from?*/
                                  QByteArray().append (message) );
    }
    
    refreshChatBox();
  }
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
** Author: Richard Baxter
**
** Refreshes the chatBox area TODO change name to refreshMainTextArea
**
****************************************************************************/

void MXitC::refreshChatBox(){


  if (currentConversation) {
    chattingToLabel->setText(currentConversation->displayName);
    mainWebView->setHtml(currentConversation->conversationHtml);
    
    QWebFrame * frame = mainWebView->page ()->currentFrame ();
    frame->setScrollBarValue(Qt::Vertical, frame->scrollBarMaximum(Qt::Vertical));
    conversationsWidget->conversationRead(currentConversation);
  }
  else {
    chattingToLabel->setText("Chatting to nobody");
    mainWebView->setHtml("");
  }
  
  
}

/****************************************************************************
**
** Author: Marc Bowes
**
** Sets up Conversations to log to a directory
**
****************************************************************************/
void MXitC::logConversations(const QDir &log)
{
  settings->setValue("logConversations", log.absolutePath());
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
  //refreshConversations();
  //refreshContacts();
  conversationsWidget->setConversationCss();
  
  addContactWidget->refresh(); /* since it contains icons*/
  /*TODO maybe make refresh a MXitDockWidget function and loop over all widgets. i.e. generalise? - rax*/
  
  conversationsWidget->setStyleSheet(theme.contact.stylesheet);
  
  /* saving theme to settings*/
  settings->setValue("themeBaseDirectory", optionsWidget->getBaseThemeDirectory());
  settings->setValue("selectedTheme", optionsWidget->getSelectedTheme());
  
}


/****************************************************************************
  _____                               __  _             
 / ___/__  ___ _  _____ _______ ___ _/ /_(_)__  ___  ___
/ /__/ _ \/ _ \ |/ / -_) __(_-</ _ `/ __/ / _ \/ _ \(_-<
\___/\___/_//_/___/\__/_/ /___/\_,_/\__/_/\___/_//_/___/
                                                        
****************************************************************************/

/****************************************************************************
**
** Author: Richard Baxter
**
** Sets the curent conversation
** NOTE:  Assumes that the conversation | contact | uniqueId is correct! 
**        error checking should be done higher up
**
****************************************************************************/

/* TODO some of this logic would be better in ensure existance(?)*/
void MXitC::setCurrentConversation(const Conversation * conversation){
  
  currentConversation = conversation;
  chatInput->setFocus(Qt::OtherFocusReason);
  refreshChatBox();
}

void MXitC::setCurrentConversation(const Contact * contact) {
  
  if (contact == NULL)
    setCurrentConversation((const Conversation *) NULL);
  else
    setCurrentConversation(ensureExistanceOfConversation(contact->contactAddress));
}


void MXitC::setCurrentConversation(const QString & uniqueId) {
  
  setCurrentConversation(ensureExistanceOfConversation(uniqueId));
}

/****************************************************************************
**
** Author: Richard Baxter
**
** returns the conversation if it exists, otehrwise creates it and returns that
**
****************************************************************************/

const Conversation * MXitC::ensureExistanceOfConversation(const QString & uniqueId) {

  const Conversation* conversation = conversations->getConversation(uniqueId);


  if (!conversation) {
    /* conversations does not exist, need to create it*/
    /* create personal (single contact) conversation */
    Conversation *newConversation = new Conversation(addressBook.contactFromAddress(uniqueId));
    newConversation->setCss(theme.chat.stylesheet);
    conversations->addConversation(newConversation);
    
    /*this *will* return a valid pointer*/
    conversation = conversations->getConversation(uniqueId);
  }
  else {
    /* conversations does exist, need to ensure it is active*/
    if (!conversation->active)
      conversations->toggleActive(uniqueId);
  }
  
  return conversation;
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
/*TODO make this a set status function*/
/*TODO disable main chat area when logged out and logging in*/
  switch (currentState) {
  
    case LOGGED_IN:  statusLabel->setText("LOGGED_IN");  break;
    case LOGGED_OUT: statusLabel->setText("LOGGED_OUT"); break;
    case LOGGING_IN: statusLabel->setText("LOGGING_IN"); break;
  
        
  }
  logWidget->logMessage("GUI:: State set to "+statusLabel->text());
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


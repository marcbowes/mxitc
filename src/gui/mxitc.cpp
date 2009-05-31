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
MXitC::MXitC(QApplication *app, MXit::Client *client) : QMainWindow (), splash(this) , environmentVariablesAreReady(false)
{
  setupUi(this);      /* from ui_dialog.h: generated from dialog.ui */
  if (QSystemTrayIcon::isSystemTrayAvailable()) {
    trayIcon = new QSystemTrayIcon(this);
  } else {
    trayIcon = NULL;
  }
  mxit = client;      /* store a copy */
  application = app;  /* store a copy */
  
  
  
  /* Getting settings from file*/
  settings = new QSettings ( "mxitc", "env", this );
  
  conversations = new MXit::Conversations(&addressBook, QDir(settings->value("logConversations").toString()));
  /*------------------------------------------------------------------------------------------*/
  /* Setting up tab-able chat window*/
  /*------------------------------------------------------------------------------------------*/
  
  
  
  conversationsWidgetsController = new ConversationsWidgetsController(theme, *mxit, *conversations, addressBook);
  
  
  conversationsTabWidget = new ConversationsTabWidget(theme, *mxit, *conversations, addressBook);
  this->centralWidget()->layout ()->addWidget (conversationsTabWidget);
   
  
  
  
  
  
  
  /*------------------------------------------------------------------------------------------*/
  /* Adding MXitDockWidgets - appendDockWidget will restore their closed& floating states as well as add thme to the necessary data structures*/
  /*------------------------------------------------------------------------------------------*/
  
  logWidget = new DockWidget::Log (this, theme);
  appendDockWidget(logWidget, Qt::RightDockWidgetArea, actionLogs);
  
  DockWidget::Debug * debugWidget = new DockWidget::Debug (this, theme);
  appendDockWidget(debugWidget,    Qt::RightDockWidgetArea, actionDebug_Variables);
  
  optionsWidget = new DockWidget::Options (this, theme, *mxit, *settings);
  appendDockWidget(optionsWidget,  Qt::RightDockWidgetArea, actionOptions);
  
  conversationsWidget = new DockWidget::Conversations (this, theme, *mxit, *conversations);
  appendDockWidget(conversationsWidget, Qt::LeftDockWidgetArea, actionConversations);
  
  contactsWidget = new DockWidget::Contacts (this, theme, *mxit, addressBook, *conversations, *optionsWidget);
  appendDockWidget(contactsWidget, Qt::LeftDockWidgetArea, actionContacts);
  
  addContactWidget = new DockWidget::AddContact (this, theme, *mxit);
  appendDockWidget(addContactWidget, Qt::LeftDockWidgetArea, actionAdd_Contact);
  
  profileWidget = new DockWidget::Profile (this, theme, *mxit);
  appendDockWidget(profileWidget, Qt::LeftDockWidgetArea, actionProfile_Settings);
  
  
  
  conversationsWidgetsController = new ConversationsWidgetsController(theme, *mxit, *conversations, addressBook);
  conversationsWidgetsController->addConversationsWidget(conversationsTabWidget);
  conversationsWidgetsController->addConversationsWidget(conversationsWidget);
  
  
  /*------------------------------------------------------------------------------------------*/
  /* Connecting new variables SIGNAL from the client to tell debugWidget to update
  /*------------------------------------------------------------------------------------------*/
  connect(mxit, SIGNAL(outgoingVariables(const VariableHash&)), debugWidget, SLOT(incomingVariableHash(const VariableHash&)));
  
  
  /*------------------------------------------------------------------------------------------*/
  /* Unsorted connects
  /*------------------------------------------------------------------------------------------*/
  
  connect(mxit, SIGNAL(environmentReady()), this, SLOT(environmentVariablesReady()));
  
  
  /*NOTE: this is contact -> conversation, not conversation -> conversation, otherwise it would go inside the controller*/
  connect(  contactsWidget, SIGNAL(outgoingConversationShowRequest ( const Contact *  )), 
            conversationsWidgetsController, SLOT(incomingConversationShowRequest( const Contact * )));
     
     
  connect(contactsWidget, SIGNAL (groupsUpdated( const QStringList & )), addContactWidget, SLOT(updateGroups(const QStringList & )));
  
            
          
  connect(&addressBook, SIGNAL(presenceToggled(const Contact*)),
          this,         SLOT(presenceToggled(const  Contact*)));
          
          
   
  /*------------------------------------------------------------------------------------------*/
  /* MenuBar Actions
  /*------------------------------------------------------------------------------------------*/
          
  /*TODO, this needs to be implemented in the client*/
  //connect(actionLogoff,  SIGNAL(triggered()), mxit, SLOT(logoff()));
  connect(actionAuto_Logon,  SIGNAL(triggered()), this, SLOT(autoLogin()));
  
  connect(actionRegister,  SIGNAL(triggered()), this, SLOT(openRegisterDialog())); /*TODO check the whole shpeel with the GUI state == REGISTERING and whatnot*/
  
  connect(actionLogon_to_MXIT, SIGNAL(triggered()), this, SLOT(openLoginDialog()));
  
  connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
  
  
  /*------------------------------------------------------------------------------------------*/
  /* Connecting various signals from the widgets to the client
  /*------------------------------------------------------------------------------------------*/
  
  connect(  mxit, SIGNAL(outgoingError(int, const QString &)), 
            this, SLOT(incomingError(int, const QString &)));
            
  connect(  mxit, SIGNAL(outgoingAction(Action)), 
            this, SLOT(incomingAction(Action))  );
            
  connect(  mxit, SIGNAL(outgoingConnectionError(const QString &)), 
            this, SLOT(incomingConnectionError(const QString &))  );
            
  connect(  mxit, SIGNAL(errorEncountered(const QString &)), 
            this, SLOT(incomingConnectionError(const QString &))  );
  

  connect(  mxit, SIGNAL(outgoingConnectionState(Network::Connection::State)), 
            this, SLOT(incomingConnectionState(Network::Connection::State))  );
  
  /*------------------------------------------------------------------------------------------*/
  /* Hooking up themeing refreshes and logging */
  /*------------------------------------------------------------------------------------------*/
  
  connect(  optionsWidget, SIGNAL(themeChanged()), this, SLOT(themeChanged()));
  connect(  optionsWidget, SIGNAL(themeChanged()), this, SLOT(setUpStatusBar()));
  connect(  optionsWidget, SIGNAL(themeChanged()), conversationsWidget, SLOT(refreshThemeing()));
  connect(  optionsWidget, SIGNAL(themeChanged()), contactsWidget, SLOT(refreshThemeing()));
  connect(  optionsWidget, SIGNAL(themeChanged()), conversationsTabWidget, SLOT(refreshThemeing()));
  
  
  /* Conversation log dir */
  connect(optionsWidget, SIGNAL(conversationLogDirectorySelected(const QDir&)),
          this,         SLOT(logConversations(const QDir&)));
  
  /*------------------------------------------------------------------------------------------*/
  /* Setting up status bar
  /*------------------------------------------------------------------------------------------*/
  
  statusLabel = new QLabel("No status set!");
  presenceComboBox = new QComboBox();
  moodComboBox = new QComboBox();
  statusbar->addPermanentWidget(presenceComboBox);
  statusbar->addPermanentWidget(moodComboBox);
  statusbar->addPermanentWidget(statusLabel);
  setStatus(LOGGED_OUT);
  
  connect(presenceComboBox, SIGNAL(currentIndexChanged (int)), this, SLOT(presenceChanged(int)));
  connect(moodComboBox, SIGNAL(currentIndexChanged (int)), this, SLOT(moodChanged(int)));
  
  /*------------------------------------------------------------------------------------------*/
  /* Loading client hash variables from QSettings and passing to client
  /*------------------------------------------------------------------------------------------*/
  
  /* this is the list of variables that are needed for authentication*/
  
  requiredToAuth.append("err");                  /* 0 = success, else failed */
  requiredToAuth.append("url");                  /* URL that should be used for the Get PID request */
  requiredToAuth.append("sessionid");            /* unique identifier to identify the session for code image answer */
  requiredToAuth.append("captcha");              /* base64 encoded image data */
  requiredToAuth.append("countries");            /* list of available countries (countrycode|countryname)
                                                  * the list of country names should be presented to the user in order to
                                                  * find the country code that should be used later on */
  requiredToAuth.append("languages");            /* list of supported languages (locale|languagename)
                                                  * the list of language names should be presented to the user and the
                                                  * corresponding locale saved by the client to be used later on */
  requiredToAuth.append("defaultCountryName");   /* country name of the country detenced from the requestors IP */
  requiredToAuth.append("defaultCountryCode");   /* country code associated with the defaultCountryName */
  requiredToAuth.append("regions");              /* a '|' seperated list of regions if requested */
  requiredToAuth.append("defaultDialingCode");   /* dialing code associated with the defaultCountryName */
  requiredToAuth.append("defaultRegion");        /* a region of the detected IP */
  requiredToAuth.append("defaultNPF");           /* the national dialing prefix for the defaultCountryName, e.g. 0 */
  requiredToAuth.append("defaultIPF");           /* the international dialing prefix for the defaultCountryName, e.g. 00 */
  requiredToAuth.append("cities");               /* NOT IMPLEMENTED YET */
  requiredToAuth.append("defaultCity");          /* the city of the detected IP */

  requiredToAuth.append("loginname");
  requiredToAuth.append("encryptedpassword");
  requiredToAuth.append("dc");
  requiredToAuth.append("soc1");
  requiredToAuth.append("http1");
  requiredToAuth.append("soc2");
  requiredToAuth.append("http2");
  
  requiredToAuth.append("locale");
  
  /* After the MXitDockWidget has been added, it attributes can be restored*/
  restoreState(settings->value("gui layout").toByteArray());
  /*------------------------------------------------------------------------------------------*/
  /*Settings restore */
  /*------------------------------------------------------------------------------------------*/
  
  if(settings->contains("mainWindowSize"))
    resize(settings->value("mainWindowSize").toSize());

  /* After the MXitDockWidget has been added, it attributes can be restored*/
  /* this function is a bit inefficient but what the hell, this happens only once pre application run*/
  //restoreState(settings->value("gui layout").toByteArray());
  
  /* normally the optionsWidget->setSelectedTheme will trigger the optionWidget's themeChanged SIGNAL which will (in a few lines) be connected to this class's themeChange SLOT
  We can't connect the themeChanged SIGNAL/SLOTs up since that would cause a QSettings save on the 'selected theme' on index 0 of the list and this->themeChanged which will overwrite the restored (correct!) QSettings value for 'selected theme' (something [TODO find out again] sets index to 0 => changes index of list => optionsWidget's loadTheme => this class's themeChanged => which overwrites the settings)*/
  themeChanged(); /* so we just call this manually since we know now the correct theme is selected*/
  setUpStatusBar();
  /* connecting widgets */
  connectWidgets();
  
  
  if (!settings->contains("wizardRun")) {
    FirstRunWizard frw(*optionsWidget);
    frw.exec();
    settings->setValue("wizardRun", true);
    
    if (frw.registerSelected()) {
      openRegisterDialog ();
    } 
    else {
      openLoginDialog ();
    }
  }
  else 
    autoLogin (optionsWidget->isAutoLogin());
    
    
  loadLayout();
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
  disconnect();

  statusbar->removeWidget(statusLabel);
  delete statusLabel;
  
  /* clean up MXitDockWidgets*/
  Q_FOREACH(const QDockWidget * dw, dockWidgets) {
    delete dw;
  }
  
  if (trayIcon)
    delete trayIcon;
  
  delete conversationsWidgetsController;
  delete conversationsTabWidget;
  
    
  /* MUST delete this last as dockwidgets might be using it in their destructors*/
  settings->sync(); /*just in case*/
  
  delete settings;
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void MXitC::autoLogin (bool autologin) {

  /* creating the variable hash*/
  VariableHash variableHash;
  
  bool hasAllVariables = true;
  
  Q_FOREACH(const QString &var, requiredToAuth) {
    /*if a variable that is needed (i.e. is in requiredToAuth) is not in settings then the auto login won't work and we need to bail out of the autologin process*/
    if (!settings->contains(var)) {
      hasAllVariables = false;
      break;
    }
    /*else we add it to the hash needed to login*/
    variableHash[var] = settings->value(var).toByteArray();
  }
  
    /*TODO sort out autologin, what if the user has deselected it in options, how do the hash variables get to the client, does the user still want an autologin but just promted or does the user want to retype in their password and captch each time?*/
    
  /*if the settings was able to load all the necessary variables, and the autologin is set to true, the autologin can commence*/
  if (autologin && hasAllVariables) {
    loggingIn();
    mxit->authenticate(variableHash);
  }
  else {
    //mxit->initialize();
    openLoginDialog(); /* will initialize automatically*/
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
** function is called when the environment variables are set
**
****************************************************************************/

void MXitC::environmentVariablesReady() {

  environmentVariablesAreReady = true;
  /*TODO make a log*///qDebug() << "environmentVariablesReady";
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

  dockWidgetToAction[dockWidget] = action;
  /* adding to vector */
  dockWidgets.append(dockWidget);
  
  /* setting up features and adding it to this class (QMainWindow)*/
  dockWidget->setFeatures (QDockWidget::AllDockWidgetFeatures);
  addDockWidget(Qt::RightDockWidgetArea, dockWidget);
  dockWidget->setVisible ( false );
    
}


/****************************************************************************
**
** Author: Richard Baxter
**
** loads all the widget's attributes
**
****************************************************************************/

void MXitC::loadLayout() {
  
  
  Q_FOREACH (QDockWidget * dockWidget, dockWidgets) {
  
    /* loading visibility, size and floating attributes*/
    dockWidget->setVisible(settings->value(QString("visible?")+dockWidget->objectName ()).toBool());
    dockWidget->setFloating(settings->value(QString("floating?")+dockWidget->objectName ()).toBool());
    
    dockWidget->resize(settings->value(QString("size?")+dockWidget->objectName ()).toSize());
  }
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void MXitC::connectWidgets() {
  
  
  Q_FOREACH (QDockWidget * dockWidget, dockWidgets) {
    /* connecting the associated action to it's toggleVisibility SLOT*/
    /* TODO still haven't figured out how to check if the widget is raised or not, right now it only toggles visibility and doesn't cycle from visible but not raised -> visible and raised -> not visible -> visible and raised*/
    connect(dockWidgetToAction.value(dockWidget), SIGNAL(triggered()), dockWidget, SLOT(toggleVisibility()));
    
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
  saveLayout();
}

/*Qt::DockWidgetArea is never actually used, it's there so this function could be connected nicely*/
void MXitC::saveLayout(Qt::DockWidgetArea area) {
  
  Q_FOREACH(const QDockWidget * dw, dockWidgets) {
  
    settings->setValue(QString("visible?")+dw->objectName (), dw->isVisible());
    settings->setValue(QString("floating?")+dw->objectName (), dw->isFloating());
    settings->setValue(QString("size?")+dw->objectName (), dw->size());
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
   __  ___             __               __  ___                              
  /  |/  /__  ___  ___/ / ___ ____  ___/ / / _ \_______ ___ ___ ___  _______ 
 / /|_/ / _ \/ _ \/ _  / / _ `/ _ \/ _  / / ___/ __/ -_|_-</ -_) _ \/ __/ -_)
/_/  /_/\___/\___/\_,_/  \_,_/_//_/\_,_/ /_/  /_/  \__/___/\__/_//_/\__/\__/ 
                                                                             
****************************************************************************/

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void MXitC::setUpStatusBar() {
  
  presenceComboBox->clear();
  moodComboBox->clear();
  
  #define ADD(x) presenceComboBox->addItem ( theme.contact.presence.pixmap(MXit::Protocol::Enumerables::Contact::Presence(MXit::Protocol::Enumerables::Contact::x)), #x, MXit::Protocol::Enumerables::Contact::Presence(MXit::Protocol::Enumerables::Contact::x));
  
  ADD(Offline);
  ADD(Online);
  ADD(Away);
  ADD(Available);
  ADD(DoNotDisturb);
  
  #undef ADD
  
  #define ADD(x) moodComboBox->addItem (#x, MXit::Protocol::Enumerables::Contact::Mood(MXit::Protocol::Enumerables::Contact::x));
  
  ADD(None);
  ADD(Angry);
  ADD(Excited);
  ADD(Grumpy);
  ADD(Happy);
  ADD(InLove);
  ADD(Invincible);
  ADD(Sad);
  ADD(Hot);
  ADD(Sick);
  ADD(Sleepy);
  
  #undef ADD
  
  
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void MXitC::presenceChanged(int index) {
  
  if(currentState == LOGGED_IN)
    mxit->setShownPresenceAndStatus(MXit::Protocol::Enumerables::Contact::Presence(presenceComboBox->itemData ( index ).toInt()));

}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void MXitC::moodChanged(int index) {
  qDebug() << "moodChanged(" << index;
  if(currentState == LOGGED_IN)
    mxit->setMood(MXit::Protocol::Enumerables::Contact::Mood(moodComboBox->itemData ( index).toInt()));

}

/****************************************************************************
   ____                    _             ___      __  _             
  /  _/__  _______  __ _  (_)__  ___ _  / _ |____/ /_(_)__  ___  ___
 _/ // _ \/ __/ _ \/  ' \/ / _ \/ _ `/ / __ / __/ __/ / _ \/ _ \(_-<
/___/_//_/\__/\___/_/_/_/_/_//_/\_, / /_/ |_\__/\__/_/\___/_//_/___/
                               /___/                                

****************************************************************************/

void MXitC::incomingConnectionState(Network::Connection::State networkState) {
  
  if (networkState == Network::Connection::CONNECTING) {
    /* TODO */
  }
  else if (networkState == Network::Connection::CONNECTED) {
    /* FIXME reconnect? */
  }
  else if (networkState == Network::Connection::DISCONNECTING) {
    /* TODO */
  }
  else if (networkState == Network::Connection::DISCONNECTED) {
    setStatus(LOGGED_OUT);/*FIXME not really logged off ...*/
  }
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
      else /* if (currentState != LOGGED_IN) */
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
        
        presenceComboBox->setCurrentIndex(1);
        moodComboBox->setCurrentIndex(0);
        
        setStatus(LOGGED_IN);
      }
      
      break;
      
    //--------------------------------------
    case LOGGED_OUT:
      logWidget->logMessage("GUI::LOGGED_OUT");
      
      if (currentState == LOGGED_OUT)
        ;/* do nothing TODO */
      else /* if (currentState != LOGGED_OUT) */
      {
        setStatus(LOGGED_OUT);
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
    case MULTIMEDIA_RECEIVED:
      dealWithMultimedia();
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
  conversationsWidgetsController->ensureExistanceAndActivationOfConversation(mxit->variableValue("contactAddress"));
  
  conversations->addMessage(  mxit->variableValue("contactAddress"),
                              mxit->variableValue("dateTime"), 
                              mxit->variableValue("type"),
                              mxit->variableValue("contactAddress"), 
                              mxit->variableValue("flags"),
                              mxit->variableValue("message"));
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Saves multimedia data
**
****************************************************************************/
void MXitC::dealWithMultimedia()
{
  QByteArray multimedia = mxit->variableValue(QString("%1_image").arg(QString(mxit->variableValue("handle"))));
  quint16 type = mxit->variableValue(QString("%1_type").arg(QString(mxit->variableValue("handle")))).toUInt();
  quint16 ttl = mxit->variableValue(QString("%1_timeToShow").arg(QString(mxit->variableValue("handle")))).toUInt();

  if (type == Protocol::Enumerables::ChunkedData::SplashImage) {
    /* File saving temporarily disabled */
    /*
    QFile file(mxit->variableValue("handle"));
    file.open(QFile::WriteOnly);
    file.write(multimedia);
    */
    splashImage.loadFromData(multimedia);
    splash.setPixmap(splashImage);
    splash.show();
    QTimer::singleShot(ttl * 1000, &splash, SLOT(close()));
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
  conversations->log = log;
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
  /*TODO maybe make refresh a MXitDockWidget function and loop over all widgets. i.e. generalise? - rax*/
  
  addContactWidget->refresh(); /* since it contains icons*/
  
  /* saving theme to settings*/
  settings->setValue("themeBaseDirectory", optionsWidget->getBaseThemeDirectory());
  settings->setValue("selectedTheme", optionsWidget->getSelectedTheme());
  
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void MXitC::presenceToggled(const MXit::Contact* contact) {
  
  Q_FOREACH (const MXit::Conversation* conversation, conversations->getInvolvements(contact)) {
    conversations->addSystemMessage(conversation->uniqueIdentifier.toAscii (), (contact->nickname + " is now "+ getPresenceString(contact->presence)).toAscii ());
  
  }
  
}

/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/
QString MXitC::getPresenceString(Protocol::Enumerables::Contact::Presence presence)
{
  using namespace Protocol::Enumerables::Contact;
  switch (presence) {
    case Offline:       return "Offline";
    case Online:        return "Online";
    case Away:          return "Away";
    case Available:     return "Available";
    case DoNotDisturb:  return "Do Not Disturb";
    case Unaffiliated:  return "Unaffiliated";
  }
  
  return "Unknown"; 
}



/****************************************************************************
**
** Author: Richard Baxter
**
** Sets the status bar text based on the state of the application
**
****************************************************************************/

void MXitC::setStatus(State newState)
{

  currentState = newState;
  
  switch (currentState) {
  
    case LOGGED_IN:  statusLabel->setText("Logged in");  
    break;
    case LOGGED_OUT: statusLabel->setText("Logged out"); 
    contactsWidget->clearList();
    break;
    case LOGGING_IN: statusLabel->setText("Logging in");
     break;
    case REGISTERING: statusLabel->setText("Registering");
     break;
  
  }
  logWidget->logMessage("GUI:: State set to "+statusLabel->text());
  
  centralwidget->setEnabled(currentState == LOGGED_IN);
  
  emit stateChanged(currentState);
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
  
  //TODO emit errorOrSomethingIfTheLoginOrRegisterDialogNeedsIt()
  
  // old code
  //if (login != NULL) {
  //  login->resetButtons();
  //}
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
  
  //qDebug() << "incomingError: " << errorString;
  if (trayIcon && trayIcon->isVisible()) {
    //qDebug() << "sending to tray";
    trayIcon->showMessage(QString("Error #%1").arg(errorCode),  errorString);
  }
  
  emit outgoingLoginRegisterError(errorString);
  
  // old code
  //if (login != NULL) {
  //  login->resetButtons();
  //}
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
  setStatus(LOGGING_IN);
}

/****************************************************************************
**
** Author: Richard Baxter
**
** tells the gui that it's state should be logging in
**
****************************************************************************/

void MXitC::registering(){
  /*FIXME, do I log off here if registering?*/
  setStatus(REGISTERING);
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void MXitC::incomingEnvironmentVariablesPing() {
  mxit->initialize();
}



/****************************************************************************
**
** Author: Richard Baxter
**
** Opens the login dialog
**
****************************************************************************/

void MXitC::openLoginDialog(){
  
  Dialog::Login login(this, mxit, settings);
  
  connect(&login, SIGNAL(pingEnvironmentVariables()), this, SLOT(incomingEnvironmentVariablesPing()));
  connect(&login, SIGNAL(loggingIn()), this, SLOT(loggingIn()));
  connect(this, SIGNAL(stateChanged(State)), &login, SLOT(incomingStateChange(State)));
  connect(this, SIGNAL(outgoingLoginRegisterError(const QString&)), &login, SLOT(incomingError(const QString&)));
  connect(this, SIGNAL(outgoingEnvironmentVariablesReady()), &login, SLOT(environmentVariablesReady()));
  connect(&login, SIGNAL(pingEnvironmentVariables()), this, SLOT(incomingEnvironmentVariablesPing()));
  
  login.exec();
  
  disconnect(&login, SIGNAL(pingEnvironmentVariables()), this, SLOT(incomingEnvironmentVariablesPing()));
  disconnect(this, SIGNAL(outgoingEnvironmentVariablesReady()), &login, SLOT(environmentVariablesReady()));
  disconnect(this, SIGNAL(outgoingLoginRegisterError(const QString&)), &login, SLOT(incomingError(const QString&)));
  disconnect(this, SIGNAL(stateChanged(State)), &login, SLOT(incomingStateChange(State)));
  disconnect(&login, SIGNAL(loggingIn()), this, SLOT(loggingIn()));
  disconnect(&login, SIGNAL(pingEnvironmentVariables()), this, SLOT(incomingEnvironmentVariablesPing()));
}


/****************************************************************************
**
** Author: Richard Baxter
**
** Opens the register dialog
**
****************************************************************************/

void MXitC::openRegisterDialog(){
  
  Dialog::Register regis(this, mxit, settings);
  
  
  connect(&regis, SIGNAL(pingEnvironmentVariables()), this, SLOT(incomingEnvironmentVariablesPing()));
  connect(&regis, SIGNAL(registering()), this, SLOT(registering()));
  connect(this, SIGNAL(stateChanged(State)), &regis, SLOT(incomingStateChange(State)));
  connect(this, SIGNAL(outgoingLoginRegisterError(const QString&)), &regis, SLOT(incomingError(const QString&)));
  connect(this, SIGNAL(outgoingEnvironmentVariablesReady()), &regis, SLOT(environmentVariablesReady()));
  connect(&regis, SIGNAL(pingEnvironmentVariables()), this, SLOT(incomingEnvironmentVariablesPing()));
  
  regis.exec();
  
  disconnect(&regis, SIGNAL(pingEnvironmentVariables()), this, SLOT(incomingEnvironmentVariablesPing()));
  disconnect(this, SIGNAL(outgoingEnvironmentVariablesReady()), &regis, SLOT(environmentVariablesReady()));
  disconnect(this, SIGNAL(outgoingLoginRegisterError(const QString&)), &regis, SLOT(incomingError(const QString&)));
  disconnect(this, SIGNAL(stateChanged(State)), &regis, SLOT(incomingStateChange(State)));
  disconnect(&regis, SIGNAL(registering()), this, SLOT(registering()));
  disconnect(&regis, SIGNAL(pingEnvironmentVariables()), this, SLOT(incomingEnvironmentVariablesPing()));
}
  
  
}

}









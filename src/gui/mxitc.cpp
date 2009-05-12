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
  mxit = client;      /* store a copy */
  application = app;  /* store a copy */
  
  login = NULL;
   
  settings = new QSettings ( "mxitc", "env", this );
  
  /* Adding MXitDockWidgets*/
  
  DockWidget::Debug * debugWidget = new DockWidget::Debug (this, theme);
  appendDockWidget(debugWidget,    Qt::RightDockWidgetArea, actionDebug_Variables);
  
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
  //connect(actionAddContact, SIGNAL(triggered()), this, SLOT(openAddContactDialog()));
  
  restoreState(settings->value("gui layout").toByteArray());
  
  optionsWidget->setBaseThemeDirectory(settings->value("themeBaseDirectory").toString());
  optionsWidget->setSelectedTheme(settings->value("selectedTheme").toString());
  themeChanged(); /* HACK? */
  
  /*TODO mxit still need addcontact slot*/
  connect(addContactWidget, 
          SIGNAL(addContact(const QString &, const QString &, const QString &, Protocol::Enumerables::Contact::Type, const QString &)), 
          mxit, 
          SLOT  (addContact(const QString &, const QString &, const QString &, Protocol::Enumerables::Contact::Type, const QString &))  );
  
  connect(chatSessionsWidget, 
          SIGNAL(contextMenuRequest(const QPoint &, const QString &)), 
          this , 
          SLOT(chatSessionsMenu(const QPoint &, const QString &))  );
          
  connect(contactsWidget, 
          SIGNAL(contextMenuRequest(const QPoint &, const QString &)), 
          this , 
          SLOT(contactsMenu(const QPoint &, const QString &))  );
  
  connect(mxit, SIGNAL(outgoingVariables(const VariableHash&)), debugWidget, SLOT(incomingVariableHash(const VariableHash&)));
  
  mainTextArea->setFocusProxy(chatInput);
  
  /*TODO integrate into QT designer (?)*/
  connect(actionLogon_to_MXIT, SIGNAL(triggered()), this, SLOT(openLoginDialog()));
  connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
  
  connect(chatInput,  SIGNAL(returnPressed ()), this, SLOT(sendMessageFromChatInput()));
  
  connect(mxit, SIGNAL(outgoingError(int, const QString &)), this, SLOT(incomingError(int, const QString &)));
  connect(mxit, SIGNAL(outgoingAction(Action)), this, SLOT(incomingAction(Action)));
  
  
  connect(chatSessionsWidget, SIGNAL(outgoingItemPressed ( QListWidgetItem *  )), this, SLOT(setCurrentChatSession( QListWidgetItem *  )));
  
  connect(optionsWidget, SIGNAL(gatewaySelected(bool)), this, SLOT(sendGateway( bool )));  

  connect(optionsWidget, SIGNAL(themeChanged()), this, SLOT(themeChanged()));
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  statusLabel = new QLabel("No status set!");
  statusbar->addPermanentWidget(statusLabel);
  setStatusBar();
  
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
  
  VariableHash variableHash;
  
  bool hasAllVariables = true;
  
  Q_FOREACH(const QString &var, variables) {
    if (!settings->contains(var)) {
      hasAllVariables = false;
      break;
    }
    variableHash[var] = settings->value(var).toByteArray();
  }
  
  if (hasAllVariables) {
    loggingIn();
    mxit->authenticate(variableHash);
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
  // nothing here
  delete settings;
  statusbar->removeWidget(statusLabel);
  delete statusLabel;
  
  
  Q_FOREACH(const QDockWidget * dw, dockWidgets) {
    delete dw;
  }
  
  
  /*for (int r = 0 ; r < mainChatArea->rowCount() ; r++){
    for (int c = 0 ; c < mainChatArea->columnCount () ; c++){
      QTableWidgetItem * item = mainChatArea->takeItem (r, c);
      if (item)
        delete item;
    }
  }*/
}

/****************************************************************************
**
** Author: Richard Baxter
**
** the chatSessionsWidget requested a context menu
** pos is relative to the chatSession dock widget
**
****************************************************************************/

void MXitC::chatSessionsMenu(const QPoint & pos, const QString& chatSessionName) {
  
  ChatSession & chatSession = chatSessions[chatSessionName];
  
  QMenu contextMenu(chatSessionName, this);
  
  #define ADD(x,y) QAction x(y, this); contextMenu.addAction(&x);
  ADD(closeChat, "Close Chat");
  #undef ADD
  
  //qDebug() << chatSessionsWidget;
 
  QAction * selection = contextMenu.exec( pos );
  
  
       if (selection == &closeChat) {
    qDebug() << "closeChat";
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
  QMenu contextMenu(nickname, this);
  
  /*FIXME!!!!!!!!!!!!!!!!!!! use QHash n stuff - rax*/
  QHash<QString, QAction*> hash;
  #define ADD(y) hash[y] = new QAction (y, this); contextMenu.addAction(hash[y]);
  
  qDebug() << contact.nickname;
  qDebug() << contact.presence;
  
  if (contact.presence == Protocol::Enumerables::Contact::Unaffiliated) {
    ADD(         "Accept");
  }
  else {
    ADD(           "Chat");
    ADD( "Change Nickname");
    ADD(    "Change Group");
    ADD(       "Send File");
    ADD(  "Remove Contact");
  }
  #undef ADD
 
  QAction * selection = contextMenu.exec( pos );
  
  
  if (contact.presence == Protocol::Enumerables::Contact::Unaffiliated) {
    if (selection == hash["Accept"]) 
    {
      /*TODO*/
    }
  }
  else 
  {
    if (selection == hash["Chat"]) 
    {
      qDebug() << "chat";
      startChatSessionWithContact(contact);
      
      /* since the user requested a chat, bring forward the chatSessions dockwindow*/
      chatSessionsWidget->raise();
      Q_FOREACH(const ChatSession& cs, chatSessions.values()) {
        qDebug() << cs.chatSessionName;
    }
    qDebug() << "";
    chatSessionsWidget->selectItem(chatSessions[contact.nickname].chatSessionName);
      
    }
    else if (selection == hash["Change Nickname"]) {
      qDebug() << "changeNickname";
    }
    else if (selection == hash["Change Group"]) {
      qDebug() << "changeGroup";
    }
    else if (selection == hash["Send File"]) {
      qDebug() << "sendFile";
    
    }
    else if (selection == hash["Remove Contact"]) {
      qDebug() << "removeContact";
      
      QMessageBox sure;
      sure.setText("Are you sure you wish to remove "+nickname);
      sure.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
      sure.setDefaultButton(QMessageBox::Cancel);
      
      sure.exec();
    }
  }
  
  Q_FOREACH(QAction * act, hash){
    delete act;
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void MXitC::startChatSessionWithContact (MXit::Contact & contact) {
  
  if (chatSessions.contains(contact.nickname)) {
    /*then just open the existing one*/ 
    setCurrentChatSession (contact.nickname); 
  }
  else {
    /* create new ChatSession */
    chatSessions[contact.nickname] = ChatSession(&contact);
    //contactsChatSession[&contact] = &chatSessions[c.nickname];
    
    ChatSession& chatSess = chatSessions[contact.nickname];
    //chatSess.incomingMessage( Message(0, "User: "+ contact.nickname));
    chatSess.unreadMessage = false;
    
    setCurrentChatSession (chatSess.chatSessionName); 
    
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

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
****************************************************************************/

void MXitC::appendDockWidget(MXitDockWidget * dockWidget, Qt::DockWidgetArea area, QAction* action){

  dockWidgets.append(dockWidget);
  
  dockWidget->setFeatures (QDockWidget::AllDockWidgetFeatures);
  addDockWidget(Qt::RightDockWidgetArea, dockWidget);
  dockWidget->setVisible ( false );
    
    
  //qDebug() << "loading "  << QString("visible?")+dockWidget->objectName ();
  dockWidget->setVisible(settings->value(QString("visible?")+dockWidget->objectName ()).toBool());
  dockWidget->setFloating(settings->value(QString("floating?")+dockWidget->objectName ()).toBool());
    
  connect(action, SIGNAL(triggered()), dockWidget, SLOT(toggleVisibility()));
  
  connect(
          dockWidget, SIGNAL(dockLocationChanged (Qt::DockWidgetArea)), 
          this, SLOT(saveLayout(Qt::DockWidgetArea)));
  connect(
          dockWidget, SIGNAL(visibilityChanged ( bool ) ), 
          this, SLOT(saveLayout( bool )));
  
  //connect(actionDebug_Variables, SIGNAL(triggered()), this, SLOT(debugToggle()));
  //connect(actionOptions, SIGNAL(triggered()), this, SLOT(optionsToggle()));
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void MXitC::saveLayout(bool b) {
  saveLayout();
}

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
****************************************************************************/

void MXitC::subscriptionsReceived(){

  
  Q_FOREACH(const QByteArray& contact, mxit->variableValue("contacts").split('\0')) 
  {
    
    if (contact == "") break; // the last split is always ""
    /*contactAddress0 \1 nickname0 \1 type0 \1 hiddenLoginname0 \1 msg0 \1 groupchatmod0\0*/
    QVector<QByteArray> fields = QVector<QByteArray>::fromList ( contact.split('\1') );
    
    qDebug() << fields;
    QString contactAddress = fields[0];
    
    bool newContact = ensureExistanceOfContact(contactAddress);
    MXit::Contact &c = contacts[contactAddress];
    
    {
      using namespace Protocol::Enumerables::Contact;
      c.contactAddress = fields[0];
      c.nickname = fields[1];
      c.type = (Type)fields[2].toUInt();
      c.presence = Unaffiliated;
      //c.hidden = fields[3].toBool();
      //c.inviteMsg = fields[4]; //FIXME
    }
    
    nicknameToContactAddress[c.nickname] = c.contactAddress; /* FIXME This whole setting variables thing appears elsewhere in code, absract it! - rax*/
    /*FIXME deal with duplicate names!!*/
    
    refreshContacts();
      /*c.group           = contactInfo[0];
      c.contactAddress  = contactInfo[1];
      c.nickname        = contactInfo[2];
      c.presence        = (Presence)contactInfo[3].toUInt();
      c.mood            = (Mood)contactInfo[5].toUInt();*/
  }
  
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void MXitC::contactsReceived(){

  /* fetch contacts */
  /*  group0 \1 contactAddress0 \1 nickname0 \1 presence0 \1 type0 \1 mood \0
      ...
      groupN \1 contactAddressN \1 nicknameN \1 presenceN \1 typeN \1 mood
  */
  
  Q_FOREACH(const QByteArray& contact, mxit->variableValue("contacts").split('\0')) 
  {
    if (contact == "") break; // the last split is always ""
    // group0 \1 contactAddress0 \1 nickname0 \1 presence0 \1 type0 \1 mood
    QVector<QByteArray> fields = QVector<QByteArray>::fromList ( contact.split('\1') );
    
    /* FIXME ugly code below... - rax */
    
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
    
    if(newContact) {
      nicknameToContactAddress[c.nickname] = c.contactAddress;
      
    }
  }
  
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

bool MXitC::ensureExistanceOfChatSession(const QString & chatSessionName) {
  if (!chatSessions.contains(chatSessionName)) {
    chatSessions[chatSessionName] = MXit::ChatSession();
    return true;
  }
  return false;
  
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void MXitC::messageReceived(){

  /*QHashIterator<QString, Contact> i(contacts);
  while (i.hasNext()) {
    i.next();
    qDebug() << i.key() << ": " << i.value().getContactAddress();
  }*/
  
  //QString nickname = contactAddressToNickname[];
  //qDebug() << "nickname = " << nickname;
  
  
  
  /*FIXME only handles single user stuff atm!*/
  QString contactAddress = mxit->variableValue("contactAddress");
  if (contacts.contains(contactAddress)) {
  
    MXit::Contact& sender = contacts[contactAddress];
    
    ensureExistanceOfChatSession(sender.nickname);
    chatSessions[sender.nickname].incomingMessage( Message(&sender, mxit->variableValue("message")) );
    
    if (currentChatSession)
      currentChatSession->unreadMessage = false;
    
    refreshChatBox();
    chatSessionsWidget->refresh(chatSessions.values());
  }
  else {
    qDebug() << "wtf unknown contact!"; 
    /*TODO handel this eror case, even though it should NEVER happen - rax*/
  }
        
}



/****************************************************************************
**
** Author: Richard Baxter
**
** Sets the curent contact the user is chatting to
**
****************************************************************************/

void MXitC::themeChanged(){
  refreshChatSessions();
  refreshContacts();
  chatSessionsWidget->setStyleSheet(theme.contact.stylesheet);
  addContactWidget->refresh();
  
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
    Q_FOREACH(const Message& m, currentChatSession->chatHistory) {
    
      /*mainChatArea->setRowCount ( mainChatArea->rowCount ()+1 );

      qint32 lastRow = mainChatArea->rowCount ()-1;

      QTableWidgetItem * nameItem = mainChatArea->item ( lastRow, 1 );
      QTableWidgetItem * chatItem = mainChatArea->item ( lastRow, 2 );
      
      qDebug() << mainChatArea->rowCount() << ":" << mainChatArea->columnCount();
      qDebug() << nameItem << ":" << chatItem;
      if (!nameItem)
        mainChatArea->setItem(lastRow, 1, new QTableWidgetItem());
      if (!chatItem)
        mainChatArea->setItem(lastRow, 2, new QTableWidgetItem());
      
      nameItem = mainChatArea->item ( lastRow, 1 );
      chatItem = mainChatArea->item ( lastRow, 2 );
      
      qDebug() << nameItem << ":" << chatItem;

      nameItem->setText ((m.sender()?m.sender()->nickname:QString("You")));
      chatItem->setText (m.message());
      qDebug() << nameItem->text() << ":" << chatItem->text();*/
      mainTextArea->append (  QString("<") +(m.sender()?m.sender()->nickname:QString("You")) + QString("> ") +m.message() );
      //nameTextArea->append ( m.sender()?m.sender()->getNickname():"You" );
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
  outgoingMessage(chatInput->text());
  chatInput->setText("");
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
  //QMessageBox error; error.setText(QString("(%1) %2").arg(errorCode).arg(errorString));
  //error.exec();
  logWidget->logMessage("GUI:: Error "+QString("(%1) %2").arg(errorCode).arg(errorString));
  
  
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
    currentChatSession->incomingMessage( Message ( 0, message) );
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


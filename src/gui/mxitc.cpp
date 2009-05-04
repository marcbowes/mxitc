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
MXitC::MXitC(QApplication *app, MXit::Client *client) : QMainWindow ( 0 ), currentState(LOGGED_OUT)
{
  setupUi(this);      /* from ui_dialog.h: generated from dialog.ui */
  mxit = client;      /* store a copy */
  application = app;  /* store a copy */
  
  login = NULL;
  statusLabel = new QLabel("No status set!");
  statusbar->addPermanentWidget(statusLabel);
  setStatusBar();
  
  /* adding the debug window */
  debugWidget = new DebugDockWidget (this);
  debugWidget->setFeatures (QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
  addDockWidget(Qt::RightDockWidgetArea, debugWidget);
  
  connect(mxit, SIGNAL(outgoingVariables(const VariableHash&)), debugWidget, SLOT(incomingVariableHash(const VariableHash&)));
  

  connect(actionLogon_to_MXIT, SIGNAL(triggered()), this, SLOT(openLoginDialog()));
  connect(actionAddContact, SIGNAL(triggered()), this, SLOT(openAddContactDialog()));
  connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
  
  connect(chatInput,  SIGNAL(returnPressed ()), this, SLOT(sendMessageFromChatInput()));
  
  connect(mxit, SIGNAL(outgoingError(int, const QString &)), this, SLOT(incomingError(int, const QString &)));
  connect(mxit, SIGNAL(outgoingAction(Action)), this, SLOT(incomingAction(Action)));
  //connect(app, SIGNAL(lastWindowClosed()), this, SLOT(showQuitDialog()));
  
  connect(contactList, SIGNAL(itemPressed ( QListWidgetItem *  )), this, SLOT(setChatBoxText( QListWidgetItem *  )));
  
  settings = new QSettings ( "mxitc", "env", this );
  
  
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
  
  Q_FOREACH(Contact* c, contactsHash) {
    delete c;
  }
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
        
        /* vector of variables that'll e saved in settings */
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
        
        QMessageBox logged_in; 
        logged_in.setText(QString("Logged in to mxit network"));
        logged_in.exec();
        
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
        
        QMessageBox logged_out; 
        logged_out.setText(QString("Logged out of mxit network"));
        logged_out.exec();
      }
      break;
      
    //--------------------------------------
    case CONTACTS_RECEIVED:
        /* TODO remove the message box */
        
        //QMessageBox msgbox; 
        //msgbox.setText(QString("Contacts received mofo!"));
        //msgbox.exec();
        
        /* fetch contacts */
        /*  group0 \1 contactAddress0 \1 nickname0 \1 presence0 \1 type0 \1 mood \0
            ...
            groupN \1 contactAddressN \1 nicknameN \1 presenceN \1 typeN \1 mood
        */
        QByteArray contacts; /* = mxit->variableValue("contacts"); FIXME*/
        
        /* manually entering data */
        contacts.append("group1");          contacts.append('\1');
        contacts.append("uniqueAddy1");     contacts.append('\1');
        contacts.append("raxter_dude");     contacts.append('\1');
        contacts.append("1");               contacts.append('\1');
        contacts.append("0");               contacts.append('\1');
        contacts.append("0");               contacts.append('\1');
        
                                            contacts.append('\0');
                                            
        contacts.append("group2");          contacts.append('\1');
        contacts.append("uniqueAddy2");     contacts.append('\1');
        contacts.append("some_other_dude"); contacts.append('\1');
        contacts.append("0");               contacts.append('\1');
        contacts.append("1");               contacts.append('\1');
        contacts.append("2");               contacts.append('\1');
        
                                            contacts.append('\0');
                                            
        contacts.append("group3");          contacts.append('\1');
        contacts.append("uniqueAddy3");     contacts.append('\1');
        contacts.append("uhh_marc");        contacts.append('\1');
        contacts.append("3");               contacts.append('\1');
        contacts.append("2");               contacts.append('\1');
        contacts.append("2");               contacts.append('\1');
        
                                            contacts.append('\0');
                                            
        contacts.append("group1");          contacts.append('\1');
        contacts.append("uniqueAddy4");     contacts.append('\1');
        contacts.append("tim_or_someone");  contacts.append('\1');
        contacts.append("3");               contacts.append('\1');
        contacts.append("3");               contacts.append('\1');
        contacts.append("3");               contacts.append('\1');
        
        //qDebug() << contacts;
        
        QByteArray contactInfo [6];
        
        int lastIndex = -1;
        bool noMoreContacts = false;
        while (!noMoreContacts) 
        {
          for (int i = 0 ; i < 6 ; i++) 
          {
            int nextIndex = contacts.indexOf ( i == 5?'\0':'\1', lastIndex + 1);
            if (nextIndex == -1) 
            {
              nextIndex = contacts.length()-1;
              noMoreContacts = true;
            }

            contactInfo [i] = contacts.mid( lastIndex+1, nextIndex - lastIndex -1 );
            //qDebug() << lastIndex;
            //qDebug() << nextIndex;
            //qDebug() << contactInfo [i];
            lastIndex = nextIndex;
          }
          
          /* FIXME ugly code below... - rax */
          Contact * contact;
          if (contactsHash.contains(QString(contactInfo [2]))) {
             contact = contactsHash[QString(contactInfo [2])];
          }
          else {
            qDebug() << "new contact";
            contact = new Contact();
                      
          }
          
          contact->setGroup(          QString(contactInfo [0]));
          contact->setContactAddress( QString(contactInfo [1]));
          contact->setNickname(       QString(contactInfo [2]));
          contact->setPresence(       QString(contactInfo [3]).toInt());
          contact->setType(           QString(contactInfo [4]).toInt());
          contact->setMood(           QString(contactInfo [5]).toInt());
                
          if (!contactsHash.contains(QString(contactInfo [2]))) { 
            contact->chatHistory.append("User: "+contact->getNickname());
            contactsHash[contact->getNickname()] = contact;     
            
          }                   
        }
        
        /* resetting contacts list*/
        contactList->clear();/* FIXME make a tree view */
        Q_FOREACH(Contact* c, contactsHash) {
          contactList->addItem( c->getNickname() );
        }
        
        
      break;
      
  
  
  }

}


/****************************************************************************
**
** Author: Richard Baxter
**
** Sets the curent contact the user is chatting to and displays their chat history in the main text area
**
****************************************************************************/

void MXitC::setChatBoxText(QListWidgetItem * item){
  //qDebug() << item->text ();
  mainTextArea->clear();
  currentContact = contactsHash[item->text ()];
  Q_FOREACH(QString s, currentContact->chatHistory) {
    mainTextArea->append ( s );
  }
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
** Handles an incoming message (received from the network controller)
**
****************************************************************************/

void MXitC::incomingMessage(const QString & message)
{
  currentContact->chatHistory.append( "Incoming: " + message );
  mainTextArea->append ( "Incoming: " + message ); /*TODO change the string to a ChatLine class or something*/
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
  QMessageBox error; error.setText(QString("(%1) %2").arg(errorCode).arg(errorString));
  error.exec();
  
  
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
  currentContact->chatHistory.append( "Outgoing: " + message );
  mainTextArea->append ( "Outgoing: " + message );
  emit incomingMessage (message); /*FIXME, this is just to test the signal, should be deleted*/
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

/****************************************************************************
**
** Author: Richard Baxter
**
** Opens the add contact dialog
**
****************************************************************************/

void MXitC::openAddContactDialog(){
  
  MXit::GUI::Dialog::AddContact addContact(this, mxit, settings);
  addContact.exec();
  
}


/****************************************************************************
**
** Author: Richard Baxter
**
** Adds a contact to the contact tree
**
****************************************************************************/
void MXitC::updateContactsList(const QVector<Contact>& contacts)
{
  
}
  
  
}

}


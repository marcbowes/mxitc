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

  connect(actionLogon_to_MXIT, SIGNAL(triggered()), this, SLOT(openLoginDialog()));
  connect(actionAddContact, SIGNAL(triggered()), this, SLOT(openAddContactDialog()));
  connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
  
  connect(chatInput,  SIGNAL(returnPressed ()), this, SLOT(sendMessageFromChatInput()));
  
  connect(mxit, SIGNAL(outgoingError(int, const QString &)), this, SLOT(incomingError(int, const QString &)));
  connect(mxit, SIGNAL(outgoingAction(Action)), this, SLOT(incomingAction(Action)));
  //connect(app, SIGNAL(lastWindowClosed()), this, SLOT(showQuitDialog()));
  
  settings = new QSettings ( "Strio", "MXit PC", this );
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
        
        StringVec variableNames;
        variableNames.append("encryptedpassword");
        variableNames.append("dc");
        variableNames.append("soc1");
        variableNames.append("http1");
        variableNames.append("soc2");
        variableNames.append("http2");
        /* saveing variables to settings */
        for (int i = 0 ; i < variableNames.size() ; i++)
          settings->setValue(variableNames[i], mxit->variableValue(variableNames[i]));
        
        currentState = LOGGED_IN;
        
        QMessageBox logged_in; 
        logged_in.setText(QString("Logged in to mxit network"));
        logged_in.exec();
      }
      
      break;
      
    //--------------------------------------
    case LOGGED_OUT:
      
      if (currentState == LOGGED_OUT)
        ;/* do nothing TODO */
      else /* if (currentState == LOGGED_IN) */
      {
        currentState = LOGGED_OUT;
        QMessageBox logged_out; 
        logged_out.setText(QString("Logged out of mxit network"));
        logged_out.exec();
      }
      break;
      
    //--------------------------------------
    case CONTACTS_RECEIVED:
        /* TODO remove the message box */
        QMessageBox msgbox; 
        msgbox.setText(QString("Contacts received mofo!"));
        msgbox.exec();
      break;
      
  
  
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
** Handles an incoming message (recieved from the network controller)
**
****************************************************************************/

void MXitC::incomingMessage(const QString & message)
{
  mainTextArea->append ( "Incoming: " + message );
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
  mainTextArea->append ( "Outgoing: " + message );
  emit incomingMessage (message); /*FIXME, this is just to test the signal, should be deleted*/
}

/****************************************************************************
**
** Author: Richard Baxter
**
** The close even so that it displays the cose ialog before closeing
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
** Opens the login dialog
**
****************************************************************************/

void MXitC::openLoginDialog(){
  
  if (settings->contains("encryptedpassword") && settings->contains("dc")) {
    
        StringVec gateways;
        gateways.append(settings->value("soc1").toString());
        gateways.append(settings->value("http1").toString());
        gateways.append(settings->value("soc2").toString());
        gateways.append(settings->value("http2").toString());
    mxit->authenticate(settings->value("cellphone").toString(), settings->value("encryptedpassword").toString(), settings->value("dc").toString(), gateways);
  }
  else 
  {
    MXit::GUI::Dialog::Login login(this, mxit, settings);
    login.exec();
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Opens the add contact dialog
**
****************************************************************************/

void MXitC::openAddContactDialog(){
  
  MXit::GUI::Dialog::AddContact addContact(this, mxit);
  addContact.exec();
  
}


/****************************************************************************
**
** Author: Richard Baxter
**
** Adds a contact to the contact tree
**
****************************************************************************/
void MXitC::addContact(const Contact& contact)
{
  
}
  
  
}

}


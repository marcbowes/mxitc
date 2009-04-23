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
MXitC::MXitC(QApplication *app, MXit::Client *client) : QMainWindow ( 0 )
{
  setupUi(this);      /* from ui_dialog.h: generated from dialog.ui */
  mxit = client;      /* store a copy */
  application = app;  /* store a copy */

  connect(actionLogon_to_MXIT, SIGNAL(triggered()), this, SLOT(openLoginDialog()));
  connect(actionAddContact, SIGNAL(triggered()), this, SLOT(openAddContactDialog()));
  connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
  
  connect(chatInput,  SIGNAL(returnPressed ()), this, SLOT(sendMessageFromChatInput()));
  
  connect(mxit, SIGNAL(outgoingError(int, const QString &)), this, SLOT(incomingError(int, const QString &)));
  //connect(app, SIGNAL(lastWindowClosed()), this, SLOT(showQuitDialog()));
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
  qDebug() << "Error: ("<<errorCode << ") - " << errorString;
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
  
  MXit::GUI::Dialog::Login login(this, mxit);
  login.exec();
  
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


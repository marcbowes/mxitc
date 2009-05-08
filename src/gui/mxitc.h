/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::Login provides a login window.
** This is an extension of the generated dialog, made with Designer.
**
****************************************************************************/

#ifndef __MXIT_GUI_MXITC_H__
#define __MXIT_GUI_MXITC_H__

#include <QMessageBox>
#include <QDialogButtonBox>
#include <QCloseEvent>
#include <QSettings>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>

#include "common/types.h"
#include "common/actions.h"
#include "mxit/client.h"
#include "gui/debugWidget.h"
#include "gui/dialogs/login.h"
#include "gui/dialogs/addContact.h"
#include "gui/contact.h"

#include "protocol/message_type.h"

#include "ui_mxitc.h"

namespace MXit
{

namespace GUI
{

class MXitC : public QMainWindow, private Ui::MXitC
{
  Q_OBJECT

  public:         /* class specific */
  
  MXitC(QApplication *app, MXit::Client *client = 0);
  ~MXitC();
  
  enum State  {LOGGED_IN, LOGGED_OUT, LOGGING_IN};

  private: /* methods */
  void outgoingMessage(const QString & message);
  
  void updateContactsList(const QVector<Contact>& contacts);
  void setStatusBar();
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public slots:
  
  void sendMessageFromChatInput();
  void incomingMessage(const QString & contactAddress, const QString & message);
  void incomingError(int errorCode, const QString & errorString);

  protected slots:
  
  virtual void closeEvent(QCloseEvent *event);

  private slots:
  
  void loggingIn();
  void openLoginDialog();
  void openAddContactDialog();
  
  void incomingAction(Action action);
  void setCurrentUser(QListWidgetItem * item);
  void refreshChatBox(); /*FIXME slot ? - rax*/
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  private:        /* variables */
  
  
  QHash<QString, QString> contactAddressToNickname; // converts contactAddresses to their unique nickname
  QHash<QString, Contact*> contactsHash; // identified by nickname (NOT contactAddress)
  
  Contact * currentContact;
  
  MXit::Client *mxit;
  QApplication *application;
  
  QSettings *settings;
  
  State currentState;
  
  DebugDockWidget * debugWidget;
 
  Dialog::Login * login;
  QLabel * statusLabel;
};

}

}
 
#endif


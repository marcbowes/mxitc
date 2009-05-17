/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::Login provides a login window.
** This is an extension of the generated dialog, made with Designer.
**
****************************************************************************/
#define VARIABLE_DEBUGGER
#ifndef __MXIT_GUI_MXITC_H__
#define __MXIT_GUI_MXITC_H__

#include <QMessageBox>
#include <QDialogButtonBox>
#include <QCloseEvent>
#include <QSettings>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextBlock>
#include <QSplashScreen>
#include <QSystemTrayIcon>
#include <QUrl>

#include "common/types.h"
#include "common/actions.h"

#include "mxit/client.h"
#include "mxit/contact.h"
#include "mxit/address_book.h"
#include "mxit/conversations.h"
#include "gui/dock_widgets/log.h"
#include "gui/dock_widgets/debug.h"
#include "gui/dock_widgets/options.h"
#include "gui/dock_widgets/conversations.h"
#include "gui/dock_widgets/contacts.h"
#include "gui/dock_widgets/add_contact.h"

#include "gui/dialogs/login.h"
#include "gui/theme.h"

#include "protocol/enumerables/chunked_data.h"
#include "protocol/enumerables/message.h"

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
  
  protected:

  void resizeEvent ( QResizeEvent * event );

  private: /* methods */
  void outgoingMessage(const QString & message);
  
  void setStatus(State newState);
  
  /* action handlers */
  //void contactsReceived();
  void messageReceived();
  //void subscriptionsReceived();
  
  const Conversation * ensureExistanceOfConversation(const QString & uniqueId);
  
  void appendDockWidget(MXitDockWidget * dockWiget, Qt::DockWidgetArea area, QAction* action);
  
  QString getPresenceString(Protocol::Enumerables::Contact::Presence presence);
  
  void connectWidgets();
 

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  public slots:
  
  void sendMessageFromChatInput();
  void incomingConnectionError(const QString & errorString);
  void incomingError(int errorCode, const QString & errorString);

  protected slots:
  
  virtual void closeEvent(QCloseEvent *event);

  private slots:
  
  void autoLogin (bool autologin = true);
  
  
  void dealWithMultimedia();
  void logConversations(const QDir &log);
  void openLoginDialog();
  void loggingIn();
  void environmentVariablesReady();
  
  void incomingAction(Action action);
  void incomingConnectionState(Network::Connection::State networkState);
  
  void setCurrentConversation(const Conversation * conversation);
  void setCurrentConversation(const Contact * contact);
  void setCurrentConversation(const QString & uniqueId);
  
  //void chatRequestedViaContact ( QListWidgetItem * item);
  //void chatRequestedViaContact ( const QString& nickname );
  
  void saveLayout(bool b);
  void saveLayout(Qt::DockWidgetArea area = Qt::NoDockWidgetArea); 
  
  void sendGatewayToClient(const QString& gateway, const QString&, const QString&);
  
  void themeChanged();
  
  void refreshChatBox(const Conversation * conversation = NULL);
  void presenceToggled(const  Contact*);
  
  
  
  private:        /* variables */
  
  const Conversation * currentConversation;
  
  AddressBook addressBook;
  Conversations *conversations;
  
  QSplashScreen splash;
  QPixmap splashImage;
  
  MXit::Client *mxit;
  QApplication *application;
  
  QSettings *settings;
  
  State currentState;
  Theme theme;
 
  Dialog::Login * login;
  QLabel * statusLabel;
  
  StringVec requiredToAuth;
  
  /* Dockable Widgets*/
  QVector<QDockWidget *> dockWidgets;
  QHash<QDockWidget *, QAction *> dockWidgetToAction;
  
  DockWidget::Conversations * conversationsWidget;
  DockWidget::Contacts * contactsWidget;
  DockWidget::Log * logWidget;
  DockWidget::Options * optionsWidget;
  DockWidget::AddContact * addContactWidget;
  
  QSystemTrayIcon *trayIcon;
  
};

}

}
 
#endif


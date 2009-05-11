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
#include <QFile>
#include <QFileInfo>
#include <QTextBlock>

#include "common/types.h"
#include "common/actions.h"
#include "mxit/client.h"

#include "gui/dock_widgets/log.h"
#include "gui/dock_widgets/debug.h"
#include "gui/dock_widgets/options.h"
#include "gui/dock_widgets/chat_sessions.h"
#include "gui/dock_widgets/add_contact.h"

#include "gui/dialogs/login.h"
#include "gui/chat_session.h"
#include "gui/theme.h"

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

  private: /* methods */
  void outgoingMessage(const QString & message);
  
  void setStatusBar();
  
  
  void contactsReceived();
  void messageReceived();
  void subscriptionsReceived();
  
  void appendDockWidget(MXitDockWidget * dockWiget, Qt::DockWidgetArea area, QAction* action);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public slots:
  
  void sendMessageFromChatInput();
  void incomingError(int errorCode, const QString & errorString);

  protected slots:
  
  virtual void closeEvent(QCloseEvent *event);

  private slots:
  
  void loggingIn();
  void openLoginDialog();
  
  void incomingAction(Action action);
  void setCurrentChatSession(QListWidgetItem * item);
  
  void saveLayout(bool b);
  void saveLayout(Qt::DockWidgetArea area = Qt::NoDockWidgetArea); 
  
  void sendGateway(bool http);
  
  void themeChanged();
  
  
  void refreshChatBox(); /*FIXME slot ? - rax*/
  void refreshChatSessions();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  private:        /* variables */
  
  
  QHash<QString, QString> nicknameToContactAddress; // converts contactAddresses to their unique nickname
  QHash<QString, ChatSession> chatSessions; // identified by chatSession name (which is nickname if it's a single person)
  QHash<QString, MXit::Contact> contactsHash; // identified by contactAddress (NOT nickname)
  
  QHash<MXit::Contact*, ChatSession*> contactsChatSession;
  
  ChatSession * currentChatSession;
  
  MXit::Client *mxit;
  QApplication *application;
  
  QSettings *settings;
  
  State currentState;
  Theme theme;
 
  Dialog::Login * login;
  QLabel * statusLabel;
  
  /* Dockable Widgets*/
  QVector<QDockWidget *> dockWidgets;
  
  DockWidget::ChatSessions * chatSessionsWidget;
  DockWidget::Log * logWidget;
  DockWidget::Options * optionsWidget;
  
};

}

}
 
#endif


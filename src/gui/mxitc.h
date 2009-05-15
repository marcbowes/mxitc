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
#include <QFile>
#include <QFileInfo>
#include <QTextBlock>
#include <QSystemTrayIcon>

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
  
  void setStatusBar();
  
  /* action handlers */
  //void contactsReceived();
  void messageReceived();
  //void subscriptionsReceived();
  
  void refreshChatBox(); /*FIXME slot ? - rax*/
  
  
  void appendDockWidget(MXitDockWidget * dockWiget, Qt::DockWidgetArea area, QAction* action);
  
  /* TODO change to a QList that is sorted by alphabetical order, starting with the "" group*/
  /* TODO soon to be moved to AddressBook*/
  //QSet<QString> getGroupSet();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  public slots:
  
  void sendMessageFromChatInput();
  void incomingConnectionError(const QString & errorString);
  void incomingError(int errorCode, const QString & errorString);

  protected slots:
  
  virtual void closeEvent(QCloseEvent *event);

  private slots:
  
  void openLoginDialog();
  void loggingIn();
  
  void incomingAction(Action action);
  
  void setCurrentConversation(Conversation * conversation);
  
  //void chatRequestedViaContact ( QListWidgetItem * item);
  //void chatRequestedViaContact ( const QString& nickname );
  
  void saveLayout(bool b);
  void saveLayout(Qt::DockWidgetArea area = Qt::NoDockWidgetArea); 
  
  void sendGateway(bool http);
  
  void themeChanged();
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  private:        /* variables */
  
  AddressBook addressBook;
  Conversations *conversations;
  
  Conversation * currentConversation;
  
  MXit::Client *mxit;
  QApplication *application;
  
  QSettings *settings;
  
  State currentState;
  Theme theme;
 
  Dialog::Login * login;
  QLabel * statusLabel;
  
  /* Dockable Widgets*/
  QVector<QDockWidget *> dockWidgets;
  
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


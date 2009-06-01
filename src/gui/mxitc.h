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
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextBlock>
#include <QSplashScreen>
#include <QSystemTrayIcon>

#include "common/types.h"
#include "common/actions.h"

#include "common.h" /*FIXME*/

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
#include "gui/dock_widgets/profile.h"

#include "gui/dialogs/login.h"
#include "gui/dialogs/register.h"

#include "gui/theme.h"
#include "gui/first_run_wizard.h"
#include "gui/conversations_widgets_controller.h"
#include "gui/conversations_tab_widget.h"

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
  
  
  signals:
  
  void stateChanged(State newState);
  
  void outgoingLoginRegisterError(const QString&);
  
  void outgoingEnvironmentVariablesReady();
  
  protected:

  void resizeEvent ( QResizeEvent * event );

  private: /* methods */
  void outgoingMessage(const QString & message);
  
  void setStatus(State newState);
  
  /* action handlers */
  //void contactsReceived();
  void messageReceived();
  //void subscriptionsReceived();
  
  void appendDockWidget(MXitDockWidget * dockWiget, Qt::DockWidgetArea area, QAction* action);
  
  QString getPresenceString(Protocol::Enumerables::Contact::Presence presence);
  
  void connectWidgets();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  public slots:
  
  void incomingConnectionError(const QString & errorString);
  void incomingError(int errorCode, const QString & errorString);

  protected slots:
  
  virtual void closeEvent(QCloseEvent *event);

  private slots:
  
  void autoLogin (bool autologin = true, bool showDialog = true);
  
  
  void dealWithMultimedia();
  void logConversations(const QDir &log);
  
  void environmentVariablesReady();
  
  void incomingAction(Action action);
  void incomingConnectionState(Network::Connection::State networkState);
  
  void loadLayout();
  
  void saveLayout(bool b);
  void saveLayout(Qt::DockWidgetArea area = Qt::NoDockWidgetArea); 
  
  void themeChanged();
  
  void presenceToggled(const  Contact*);
  
  void incomingEnvironmentVariablesPing();
  
  void openLoginDialog();
  void openRegisterDialog();
  
  void loggingIn();
  void registering();
 
  void setUpStatusBar();
  void refreshStatusBar();
  
  void presenceChanged(int index);
  void moodChanged(int index);
  
  private:        /* variables */
  
  //const Conversation * currentConversation;
  
  AddressBook addressBook;
  Conversations *conversations;
  
  QSplashScreen splash;
  QPixmap splashImage;
  
  MXit::Client *mxit;
  QApplication *application;
  
  QSettings *settings;
  
  State currentState;
  Theme theme;
 
  /* status bar stuff TODO - put in own class*/
  QLabel * statusLabel;
  QComboBox * presenceComboBox;
  QComboBox * moodComboBox;
  
  StringVec requiredToAuth;
  
  
  bool environmentVariablesAreReady;
  
  
  ConversationsWidgetsController * conversationsWidgetsController;
  ConversationsTabWidget * conversationsTabWidget;
  
  
  /* Dockable Widgets*/
  QVector<QDockWidget *> dockWidgets;
  QHash<QDockWidget *, QAction *> dockWidgetToAction;
  
  DockWidget::Conversations * conversationsWidget;
  DockWidget::Contacts * contactsWidget;
  DockWidget::Log * logWidget;
  DockWidget::Options * optionsWidget;
  DockWidget::AddContact * addContactWidget;
  DockWidget::Profile * profileWidget;
  
  QSystemTrayIcon *trayIcon;
  
};

}

}
 
#endif


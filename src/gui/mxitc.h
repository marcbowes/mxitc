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

#include "common/types.h"
#include "common/actions.h"
#include "mxit/client.h"
#include "gui/dialogs/login.h"
#include "gui/dialogs/addContact.h"
#include "gui/contact.h"

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
  
  enum State  {LOGGED_IN, LOGGED_OUT};

  private: /* methods */
  void outgoingMessage(const QString & message);
  
  /* contact specific methods TODO should be delegated to it's own class*/
  void addContact(const Contact& contact);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public slots:
  
  void sendMessageFromChatInput();
  void incomingMessage(const QString & message);
  void incomingError(int errorCode, const QString & errorString);

  protected slots:
  
  virtual void closeEvent(QCloseEvent *event);

  private slots:
  
  void openLoginDialog();
  void openAddContactDialog();
  
  void incomingAction(Action action);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  private:        /* variables */
  
  
  
  MXit::Client *mxit;
  QApplication *application;
  
  QSettings *settings;
  
  State currentState;
};

}

}
 
#endif


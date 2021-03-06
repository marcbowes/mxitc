/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::Dialog::Login provides a login window.
** This is an extension of the generated dialog, made with Designer.
**
****************************************************************************/

#ifndef __MXIT_GUI_DIALOG_LOGIN_H__
#define __MXIT_GUI_DIALOG_LOGIN_H__

#include "mxit/client.h"
#include "gui/mxit_dialog.h"
#include "gui/widgets/login_settings.h"

#include "ui_login.h"

#include "common/gui_states.h"

namespace MXit
{

namespace GUI
{

namespace Dialog
{


class Login : public MXitDialog, private Ui::Login
{
  Q_OBJECT

  public: /* class specific */
  
  Login(QWidget *parent = 0, MXit::Client *client = 0, QSettings* settings = 0);
  ~Login();
  
  public: /* methods */
  
  void resetButtons();
  
  signals:
  
  void loggingIn();
  void pingEnvironmentVariables();
  
  public slots:
  void exec();
  
  void incomingStateChange(GuiState newState);
  
  void environmentVariablesReady();

  private slots:
  
  void checkIfLoginClickable(const QString &text);
    
  void incomingError(const QString &text);
  
  void login();
  
  private:
  
  Widget::LoginSettings* loginWidget;
  
};

} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */
 
#endif


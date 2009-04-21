/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::Login provides a login window.
** This is an extension of the generated dialog, made with Designer.
**
****************************************************************************/

#ifndef __MXIT_GUI_LOGIN_H__
#define __MXIT_GUI_LOGIN_H__

#include <QMessageBox>

#include "mxit/client.h"

#include "ui_login.h"

namespace MXit
{

namespace GUI
{

class Login : public QMainWindow, private Ui::Login
{
  Q_OBJECT

  public:         /* class specific */
  
  Login(QApplication *app, MXit::Client *client);
  ~Login();

  private slots:
  
  void captchaReceived(const QByteArray &captcha);
  void captchaChanged(const QString &text);
  void cellphoneChanged(const QString &text);
  void error(const QString &text);
  void login();
  void passwordChanged(const QString &text);
  
  private:        /* variables */
  
  MXit::Client *mxit;
};

}

}
 
#endif


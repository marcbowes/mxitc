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

#include <QMessageBox>
#include <QSettings>

#include "mxit/client.h"

#include "ui_login.h"

namespace MXit
{

namespace GUI
{

namespace Dialog
{

class Login : public QDialog, private Ui::Login
{
  Q_OBJECT

  public:         /* class specific */
  
  Login(QWidget *parent = 0, MXit::Client *client = 0, QSettings* settings = 0);
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
  
  QSettings *settings;
};

} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */
 
#endif

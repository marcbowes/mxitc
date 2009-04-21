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

#include "mxit/client.h"
#include "gui/login.h"

#include "ui_mxitc.h"

namespace MXit
{

namespace GUI
{

class MXitC : public QMainWindow, private Ui::MXitC
{
  Q_OBJECT

  public:         /* class specific */
  
  MXitC(QApplication *app, MXit::Client *client);
  ~MXitC();

  private slots:
  
  void openLoginWindow();
  /*void captchaReceived(const QByteArray &captcha);
  void captchaChanged(const QString &text);
  void cellphoneChanged(const QString &text);
  void error(const QString &text);
  void login();
  void passwordChanged(const QString &text);*/
  
  private:        /* variables */
  
  MXit::Client *mxit;
  
  QApplication *parentApp;
};

}

}
 
#endif


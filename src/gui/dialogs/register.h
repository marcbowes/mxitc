/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::Dialog::Login provides a login window.
** This is an extension of the generated dialog, made with Designer.
**
****************************************************************************/

#ifndef __MXIT_GUI_DIALOG_REGISTER_H__
#define __MXIT_GUI_DIALOG_REGISTER_H__

#include "mxit/client.h"
#include "gui/mxit_dialog.h"

#include "ui_register.h"

#include "gui/common.h" /*FIXME*/

namespace MXit
{

namespace GUI
{

namespace Dialog
{

class Register : public MXitDialog, private Ui::Register
{
  Q_OBJECT

  public:         /* class specific */
  
  Register(QWidget *parent = 0, MXit::Client *client = 0, QSettings* settings = 0);
  ~Register();
  
  public: /* methods */
  
  void resetButtons();
  
  signals:
  
  void registering();
  void pingEnvironmentVariables();
  
  public slots:
  
  void exec();
  
  void incomingStateChange(State newState);

  private slots:
  
  void environmentVariablesReady();
  
  void checkIfRegisterClickable(const QString &text);
  
  void error(const QString &text);
  
  void signUp();
  
};

} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */
 
#endif

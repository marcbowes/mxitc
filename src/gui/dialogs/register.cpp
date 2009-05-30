/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "register.h"

namespace MXit
{

namespace GUI
{

namespace Dialog
{

/****************************************************************************
**
** Author: Marc Bowes
**
** Dialog constructor
**
** ==== Arguments
** - app: owned by main.cpp
** - client: owned by main.cpp
**
****************************************************************************/
Register::Register(
              QWidget *parent, 
              MXit::Client *client, 
              QSettings* settings) : MXitDialog (parent, client, settings)
{
  setupUi(this);      /* from ui_dialog.h: generated from dialog.ui */
  
  /* add in an image to display the login CAPTCHA */
  //mxit->initialize(); /* NOTE moved to mxitc.cpp*/
    
  /* enable/disable 'Login' based on the validity of the user inputs */
  connect(cellphone, SIGNAL(textChanged(const QString &)), 
          this, SLOT(checkIfRegisterClickable(const QString &)));
  connect(password, SIGNAL(textChanged(const QString &)), 
          this, SLOT(checkIfRegisterClickable(const QString &)));
  connect(captchaLineEdit, SIGNAL(textChanged(const QString &)), 
          this, SLOT(checkIfRegisterClickable(const QString &)));
  
  /* when 'Login' is clicked, or the user presses return */
  connect(cellphone, SIGNAL(returnPressed()), this, SLOT(signUp()));
  connect(password, SIGNAL(returnPressed()), this, SLOT(signUp()));
  connect(captchaLineEdit, SIGNAL(returnPressed()), this, SLOT(signUp()));
  connect(registerButton, SIGNAL(released()), this, SLOT( signUp() ));
  connect(cancelButton, SIGNAL(released()), this, SLOT(reject ()));
  
  /* when a CAPTCHA is received from the MXit gateway, display it */
  connect(mxit, SIGNAL(environmentReady()), this, SLOT(environmentVariablesReady()));
  
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Dialog deconstructor
**
****************************************************************************/
Register::~Register()
{
  disconnect();
}

/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/

void Register::exec() {

  emit pingEnvironmentVariables();
  
  QDialog::exec();
}

/****************************************************************************
**
** Author: Richard Baxter
**
** this SLOT is triggered when any of the text in the lineEdits are changed
**
****************************************************************************/

void Register::checkIfRegisterClickable(const QString &text) {
  /*TODO, more accurate checks*/
  registerButton->setDisabled(cellphone->text().isEmpty());
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Register::incomingStateChange(State newState) {
  if (newState == LOGGED_IN) {
    qDebug() << "Register::incomingStateChange(LOGGED_IN)";
  }
  else if (newState == LOGGING_IN) {
  }
  else if (newState == REGISTERING) {
  }
  else if (newState == LOGGED_OUT) {
    qDebug() << "Register::incomingStateChange(LOGGED_OUT)";
  }

}

/****************************************************************************
**
** Author: Richard Baxter
**
** this SLOT is triggered when the client receives the environment variables
**
****************************************************************************/
/*TODO gray out everything but cancle button until environment variables have been received*/
void Register::environmentVariablesReady()
{
  /* captcha */
  QByteArray captcha = QByteArray::fromBase64(mxit->variableValue("captcha"));
  QImage captchaImage;
  captchaImage.loadFromData(captcha);
  captchaLabel->setPixmap(QPixmap::fromImage(captchaImage));
  
  captchaLineEdit->setEnabled(true);
  captchaLineEdit->setText("");
}


/****************************************************************************
**
** Author: Marc Bowes
**
** this SLOT is triggered when the client reports an error
**
****************************************************************************/
void Register::error(const QString &text)
{
  QMessageBox error; error.setText(text);
  error.exec();
}

/****************************************************************************
**
** Author: Marc Bowes
**
** this SLOT is triggered by pressing 'Register' or typing return
**
****************************************************************************/
void Register::signUp()
{
  if (!captchaLineEdit->text().isEmpty()) { /*FIXME more stringent test*/
    registerButton->setDisabled(true);
    registerButton->setText("Registering..");
    
    VariableHash variables;
    /*FIXME what must go in variables !!*/
    mxit->signup(cellphone->text().toLatin1(), password->text().toLatin1(),captchaLineEdit->text().toLatin1(), variables);

    emit registering();
    
  }
}


/****************************************************************************
**
** Author: Richard Baxter
**
** this resets the buttons for relogin
**
****************************************************************************/
void Register::resetButtons()
{
  registerButton->setDisabled(false);
  registerButton->setText("Register");
}



} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */


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
  
  /*TODO ensure user is logged out before registering!*/
  
  profileSettings = new Widget::ProfileSettings(this);
  profileSettingWidget->layout()->addWidget(profileSettings);
  
  profileSettings->passwordLabel->hide();
  profileSettings->passwordLineEdit->hide();
  profileSettings->hiddenLoginNameCheckBox->hide();
  
  loginWidget = new Widget::LoginSettings(this);
  loginHoldWidget->layout()->addWidget(loginWidget);
    
  /* enable/disable 'Reg5ster' based on the validity of the user inputs */
  connect(loginWidget, SIGNAL(lineEditChanged(const QString &)), 
          this, SLOT(checkIfRegisterClickable(const QString &)));
  
  /* when 'Login' is clicked, or the user presses return */
  connect(loginWidget, SIGNAL(returnPressed()), this, SLOT(signUp()));
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
  delete profileSettings;
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
  registerButton->setEnabled(loginWidget->isInputValid() /*&& profileSetting->isValid TODO TODO*/);
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

void Register::environmentVariablesReady()
{
  loginWidget->setCaptcha(mxit->variableValue("captcha"));
  loginWidget->setLanguagesAndCountries(mxit->variableValue("languages"), mxit->variableValue("countries"), mxit->variableValue("defaultCountryCode"));
}


/****************************************************************************
**
** Author: Marc Bowes
**
** this SLOT is triggered when the client reports an error
**
****************************************************************************/
void Register::incomingError(const QString &text)
{
  QMessageBox error; error.setText(text);
  error.exec();
  resetButtons();
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
  if (loginWidget->isInputValid() /*&& profileSetting->isValid TODO TODO*/) {
    registerButton->setDisabled(true);
    registerButton->setText("Registering..");
    
    VariableHash variables;
    variables["name"] = profileSettings->nicknameLineEdit->text().toLatin1();
    variables["dateofbirth"] = profileSettings->dateEdit->date ().toString(Qt::ISODate).toLatin1();
    variables["gender"] = QString::number(profileSettings->genderComboBox->currentIndex()).toLatin1();
    variables["locale"] = loginWidget->languageComboBox->itemData(loginWidget->languageComboBox->currentIndex ()).toByteArray(); /*language code - locale*/
    variables["cc"] = loginWidget->countriesComboBox->itemData(loginWidget->countriesComboBox->currentIndex ()).toByteArray().replace('-', '_'); /*country code*/
    
    mxit->signup(loginWidget->cellphone->text().toLatin1(), loginWidget->password->text().toLatin1(),loginWidget->captchaLineEdit->text().toLatin1(), variables);

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
  emit pingEnvironmentVariables();
}



} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */


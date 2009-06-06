/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "login.h"

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
Login::Login(
              QWidget *parent, 
              MXit::Client *client, 
              QSettings* settings) : MXitDialog (parent, client, settings)
{
  setupUi(this);      /* from ui_dialog.h: generated from dialog.ui */
  
  
  loginWidget = new Widget::LoginSettings(this);
  
  loginHoldWidget->layout()->addWidget(loginWidget);
    
  
  connect(loginWidget, SIGNAL(lineEditChanged(const QString &)), 
          this, SLOT(checkIfLoginClickable(const QString &)));
          
  /* when 'Login' is clicked, or the user presses return */
  connect(loginWidget, SIGNAL(returnPressed()), this, SLOT(login()));
  connect(loginButton, SIGNAL(released()), this, SLOT(login()));
  
  connect(cancelButton, SIGNAL(released()), this, SLOT(reject ()));
  
  /* when a CAPTCHA is received from the MXit gateway, display it */
  connect(mxit, SIGNAL(environmentReady()), this, SLOT(environmentVariablesReady()));
  
  if(settings->contains ("cellphone"))
    loginWidget->cellphone->setText(settings->value("cellphone").toString());
  
}

/****************************************************************************
**
** Author: Marc Bowes
**
** Dialog deconstructor
**
****************************************************************************/
Login::~Login()
{
  disconnect();
  delete loginWidget;
}



/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/

void Login::exec() {

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

void Login::checkIfLoginClickable(const QString &text) {

  loginButton->setEnabled(loginWidget->isInputValid());
}


/****************************************************************************
**
** Author: Richard Baxter
**
** this SLOT is triggered when the client receives the environment variables
**
****************************************************************************/

void Login::environmentVariablesReady()
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
void Login::incomingError(const QString &text)
{
  QMessageBox error; error.setText(text);
  error.exec();
}
/****************************************************************************
**
** Author: Marc Bowes
**
** this SLOT is triggered by pressing 'Login' or typing return
**
****************************************************************************/
void Login::login()
{
  if (loginWidget->isInputValid()) {
    loginButton->setDisabled(true);
    loginButton->setText("Logging in..");
    
    VariableHash variables;
    variables["locale"] = loginWidget->languageComboBox->itemData(loginWidget->languageComboBox->currentIndex ()).toByteArray(); /*language code - locale*/
    variables["cc"] = loginWidget->countriesComboBox->itemData(loginWidget->countriesComboBox->currentIndex ()).toByteArray().replace('-', '_'); /*country code*/
    
    mxit->login(loginWidget->cellphone->text().toLatin1(), loginWidget->password->text().toLatin1(),loginWidget->captchaLineEdit->text().toLatin1(), variables);
    
    emit loggingIn();
    
    settings->setValue("locale", variables["locale"]);
    settings->setValue("cellphone", loginWidget->cellphone->text());
    settings->sync();
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Login::incomingStateChange(GuiState newState) {
  if (newState == LOGGED_IN) {
    close(); 
  }
  else if (newState == LOGGING_IN) {
  }
  else if (newState == LOGGED_OUT) {
    resetButtons();
  }

}

/****************************************************************************
**
** Author: Richard Baxter
**
** this resets the buttons for relogin
**
****************************************************************************/
void Login::resetButtons()
{
  loginButton->setDisabled(false);
  loginButton->setText("Login");
  
  emit pingEnvironmentVariables();
}


} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */


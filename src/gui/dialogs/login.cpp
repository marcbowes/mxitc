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
  
    
  /* enable/disable 'Login' based on the validity of the user inputs */
  connect(cellphone, SIGNAL(textChanged(const QString &)), 
          this, SLOT(checkIfLoginClickable(const QString &)));
  connect(password, SIGNAL(textChanged(const QString &)), 
          this, SLOT(checkIfLoginClickable(const QString &)));
  connect(captchaLineEdit, SIGNAL(textChanged(const QString &)), 
          this, SLOT(checkIfLoginClickable(const QString &)));
  
  /* when 'Login' is clicked, or the user presses return */
  connect(cellphone, SIGNAL(returnPressed()), this, SLOT(login()));
  connect(password, SIGNAL(returnPressed()), this, SLOT(login()));
  connect(captchaLineEdit, SIGNAL(returnPressed()), this, SLOT(login()));
  
  connect(loginButton, SIGNAL(released()), this, SLOT(login()));
  
  connect(cancelButton, SIGNAL(released()), this, SLOT(reject ()));
  
  /* when a CAPTCHA is received from the MXit gateway, display it */
  connect(mxit, SIGNAL(environmentReady()), this, SLOT(environmentVariablesReady()));
  
  if(settings->contains ("cellphone"))
    cellphone->setText(settings->value("cellphone").toString());
  
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
  /*TODO, more accurate checks*/
  loginButton->setDisabled(cellphone->text().isEmpty());
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
  /* captcha */
  QByteArray captcha = QByteArray::fromBase64(mxit->variableValue("captcha"));
  QImage captchaImage;
  captchaImage.loadFromData(captcha);
  captchaLabel->setPixmap(QPixmap::fromImage(captchaImage));
  
  captchaLineEdit->setEnabled(true);
  captchaLineEdit->setText("");
  
  /* countries */
  QList<QByteArray> countriesList = mxit->variableValue("countries").split(',');
  
  Q_FOREACH(QByteArray s, countriesList) {
    QList<QByteArray> split = s.split('|');
    countriesComboBox->addItem( split.back(), split.front());
  }
  countriesComboBox->setCurrentIndex ( countriesComboBox->findData (QString(mxit->variableValue("defaultCountryCode"))) );
  
  /* languages */
  QList<QByteArray> languagesList = mxit->variableValue("languages").split(',');
  
  Q_FOREACH(QByteArray s, languagesList) {
    QList<QByteArray> split = s.split('|');
    languageComboBox->addItem( split.back(), split.front());
  }
  languageComboBox->setCurrentIndex ( languageComboBox->findData ("en") );
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
  if (!captchaLineEdit->text().isEmpty()) { /*FIXME more stringent test*/
    loginButton->setDisabled(true);
    loginButton->setText("Logging in..");
    
    VariableHash variables;
    variables["locale"] = languageComboBox->itemData(languageComboBox->currentIndex ()).toByteArray(); /*language code - locale*/
    variables["cc"] = countriesComboBox->itemData(countriesComboBox->currentIndex ()).toByteArray().replace('-', '_'); /*country code*/
    
    mxit->login(cellphone->text().toLatin1(), password->text().toLatin1(),captchaLineEdit->text().toLatin1(), variables);
    
    emit loggingIn();
    
    settings->setValue("locale", variables["locale"]);
    settings->setValue("cellphone", cellphone->text());
    settings->sync();
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Login::incomingStateChange(State newState) {
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


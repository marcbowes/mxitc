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
  
  /* add in an image to display the login CAPTCHA */
  //mxit->initialize(); /* NOTE moved to mxitc.cpp*/
    
  /* enable/disable 'Login' based on the validity of the user inputs */
  connect(cellphone, SIGNAL(textChanged(const QString &)), 
          this, SLOT(cellphoneChanged(const QString &)));
  connect(password, SIGNAL(textChanged(const QString &)), 
          this, SLOT(passwordChanged(const QString &)));
  connect(captcha, SIGNAL(textChanged(const QString &)), 
          this, SLOT(captchaChanged(const QString &)));
  
  /* when 'Login' is clicked, or the user presses return */
  connect(cellphone, SIGNAL(returnPressed()), this, SLOT(login()));
  connect(password, SIGNAL(returnPressed()), this, SLOT(login()));
  connect(captcha, SIGNAL(returnPressed()), this, SLOT(login()));
  connect(loginButton, SIGNAL(released()), this, SLOT(login()));
  /*FIXME*/
  connect(registerButton, SIGNAL(released()), this, SLOT(login(true)));
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
  // nothing here
}


/****************************************************************************
**
** Author: Marc Bowes
**
** this SLOT is triggered when cellphone input text is changed
**
****************************************************************************/
void Login::captchaChanged(const QString &text)
{
  loginButton->setDisabled(text.isEmpty() ? true : false);
}


/****************************************************************************
**
** Author: Richard Baxter
**
** this SLOT is triggered when the client receives the environment variables
**
****************************************************************************/
/*TODO gray out everything but cancle button until environment variables have been received*/
void Login::environmentVariablesReady()
{
  /* captcha */
  QByteArray captcha = QByteArray::fromBase64(mxit->variableValue("captcha"));
  QImage captchaImage;
  captchaImage.loadFromData(captcha);
  captchaLabel->setPixmap(QPixmap::fromImage(captchaImage));
  
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
** this SLOT is triggered when cellphone input text is changed
**
****************************************************************************/
void Login::cellphoneChanged(const QString &text)
{
  registerButton->setDisabled(text.isEmpty() ? true : false);
  loginButton->setDisabled(text.isEmpty() ? true : false);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** this SLOT is triggered when the client reports an error
**
****************************************************************************/
void Login::error(const QString &text)
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
void Login::login(bool register_)
{
  if (!captcha->text().isEmpty()) {
    loginButton->setDisabled(true);
    registerButton->setDisabled(true);
    loginButton->setText("Logging in..");
    
    VariableHash variables;
    variables["locale"] = languageComboBox->itemData(languageComboBox->currentIndex ()).toByteArray(); /*language code - locale*/
    variables["cc"] = countriesComboBox->itemData(countriesComboBox->currentIndex ()).toByteArray().replace('-', '_'); /*country code*/
    
    /*TODO does successful registration mean it logs you in automatically*/
    if (register_) {
      /*TODO wait for implementation*/
      //mxit->register(cellphone->text().toLatin1(), password->text().toLatin1(),captcha->text().toLatin1(), variables);
    }
    else {
      mxit->login(cellphone->text().toLatin1(), password->text().toLatin1(),captcha->text().toLatin1(), variables);
    }
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
** this resets the buttons for relogin
**
****************************************************************************/
void Login::resetButtons()
{
  registerButton->setDisabled(false);
  loginButton->setDisabled(false);
  loginButton->setText("Login");
}




/****************************************************************************
**
** Author: Marc Bowes
**
** this SLOT is triggered when password input text is changed
**
****************************************************************************/
void Login::passwordChanged(const QString &text)
{
  registerButton->setDisabled(text.isEmpty() ? true : false);
  loginButton->setDisabled(text.isEmpty() ? true : false);
}


} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */


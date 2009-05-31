

#include "login_settings.h"




namespace MXit
{

namespace GUI
{

namespace Widget
{

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

LoginSettings::LoginSettings(QWidget* parent) : QWidget(parent) {
  setupUi(this);
  /* enable/disable 'Login' based on the validity of the user inputs */
  connect(cellphone, SIGNAL(textChanged(const QString &)), 
          this, SIGNAL(lineEditChanged(const QString &)));
  connect(password, SIGNAL(textChanged(const QString &)), 
          this, SIGNAL(lineEditChanged(const QString &)));
  connect(captchaLineEdit, SIGNAL(textChanged(const QString &)), 
          this, SIGNAL(lineEditChanged(const QString &)));
          
          
  connect(cellphone, SIGNAL(returnPressed()), this, SIGNAL(returnPressed()));
  connect(password, SIGNAL(returnPressed()), this, SIGNAL(returnPressed()));
  connect(captchaLineEdit, SIGNAL(returnPressed()), this, SIGNAL(returnPressed()));
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

LoginSettings::~LoginSettings() {

}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

bool LoginSettings::isInputValid() {
  return !cellphone->text().isEmpty(); /*TODO more stringent*/
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void LoginSettings::setCaptcha(const QByteArray& dataCaptcha) {

  QByteArray captcha = QByteArray::fromBase64(dataCaptcha);
  QImage captchaImage;
  captchaImage.loadFromData(captcha);
  captchaLabel->setPixmap(QPixmap::fromImage(captchaImage));
  
  captchaLineEdit->setEnabled(true);
  captchaLineEdit->setText("");

}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void LoginSettings::setLanguagesAndCountries(const QByteArray& languages, const QByteArray& countries, const QByteArray& defaultCountry) {

  /* countries */
  QList<QByteArray> countriesList = countries.split(',');
  
  Q_FOREACH(QByteArray s, countriesList) {
    QList<QByteArray> split = s.split('|');
    countriesComboBox->addItem( split.back(), split.front());
  }
  countriesComboBox->setCurrentIndex ( countriesComboBox->findData (QString(defaultCountry)));
  
  /* languages */
  QList<QByteArray> languagesList = languages.split(',');
  
  Q_FOREACH(QByteArray s, languagesList) {
    QList<QByteArray> split = s.split('|');
    languageComboBox->addItem( split.back(), split.front());
  }
  languageComboBox->setCurrentIndex ( languageComboBox->findData ("en") );

}



} /* end of Widget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */








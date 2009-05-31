/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Richard Baxter, 2009
**
****************************************************************************/

#ifndef __MXIT_GUI_WIDGET_LOGINSETTINGS_H__
#define __MXIT_GUI_WIDGET_LOGINSETTINGS_H__

#include <QWidget>
#include "ui_login_settings.h"

namespace MXit
{

namespace GUI
{

namespace Widget
{

class LoginSettings : public QWidget, public Ui::LoginWidget
{
  Q_OBJECT
  
  public: /*class specific */

  LoginSettings(QWidget* parent = 0);
  ~LoginSettings();
  
  signals:
  
  void lineEditChanged(const QString&);
  void returnPressed();
  
  public: /* methods */

  bool isInputValid();
  void setCaptcha(const QByteArray& captcha);
  void setLanguagesAndCountries(
                                  const QByteArray& languages, 
                                  const QByteArray& countries, 
                                  const QByteArray& defaultCountry  );
  

};

} /* end of Widget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif



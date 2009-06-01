
#include "register_gateway.h"

namespace MXit
{

namespace GUI
{

namespace Dialog
{

RegisterGateway::RegisterGateway(const QString& gatewayLabelText, bool onlyShowUsername, QWidget *parent) : QDialog(parent) {
  setupUi(this);
  if (onlyShowUsername) {
    passwordLabel->hide();
    passwordLineEdit->hide();
  }
  
  //gatewayLabel->setText(QString(gatewayLabelText));
  
    
}


RegisterGateway::~RegisterGateway(){


}


QString RegisterGateway::getPassword() const {
  return passwordLineEdit->text();
}

QString RegisterGateway::getUsername() const {
  return usernameLineEdit->text();
}
  

} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



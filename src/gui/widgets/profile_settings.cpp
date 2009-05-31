
#include "profile_settings.h"

namespace MXit
{

namespace GUI
{

namespace Widget
{

ProfileSettings::ProfileSettings(bool deactivatePassword, QWidget* parent) {
  setupUi(this);
  
  if (deactivatePassword) {
    passwordLabel->hide();
    passwordLineEdit->hide();
  }
  
}


ProfileSettings::~ProfileSettings(){
  
  
}


} /* end of Widget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */






#include "profile_settings.h"

namespace MXit
{

namespace GUI
{

namespace Widget
{

ProfileSettings::ProfileSettings(QWidget* parent) {
  setupUi(this);
  
  
  connect(nicknameLineEdit, SIGNAL(textChanged(const QString &)), 
          this, SLOT(emitFieldChanged(const QString &)));
  connect(passwordLineEdit, SIGNAL(textChanged(const QString &)), 
          this, SLOT(emitFieldChanged(const QString &)));
  connect(dateEdit, SIGNAL(dateChanged ( const QDate & )), 
          this, SLOT(emitFieldChanged( const QDate &)));
  connect(genderComboBox, SIGNAL(currentIndexChanged(const QString &)), 
          this, SLOT(emitFieldChanged(const QString &)));
}


ProfileSettings::~ProfileSettings(){
  
  
}

void ProfileSettings::emitFieldChanged(const QDate& nothing) {
  emit fieldChanged();
}

void ProfileSettings::emitFieldChanged(const QString& nothing) {
  emit fieldChanged();
}

} /* end of Widget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */





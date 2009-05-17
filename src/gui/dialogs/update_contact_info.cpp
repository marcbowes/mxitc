
#include "update_contact_info.h"

namespace MXit
{

namespace GUI
{

namespace Dialog
{


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
UpdateContactInfo::UpdateContactInfo(const QString & nickname, const QMap<QString, bool>& groups, const QString& currentGroup, QWidget *parent) :
 QDialog(parent)
{
  setupUi(this);
  
  nicknameLineEdit->setText(nickname);
  
  Q_FOREACH(QString s, groups.keys()) {
    groupComboBox->addItem ( s );
  }
  
  groupComboBox->setCurrentIndex  ( groupComboBox->findText(currentGroup) );
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
UpdateContactInfo::~UpdateContactInfo(){}
  
  

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
  
QString UpdateContactInfo::getGroup() const{
  return groupComboBox->currentText ();
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

QString UpdateContactInfo::getNickname()  const{
  return nicknameLineEdit->text();
}

} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#include "update_group_name.h"

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
UpdateGroupName::UpdateGroupName(const QStringList& groups, const QString& currentGroup, QWidget *parent) :
 QDialog(parent)
{
  setupUi(this);
  
  groupComboBox->addItems ( groups );
  groupComboBox->setCurrentIndex  ( groupComboBox->findText(currentGroup) );
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
UpdateGroupName::~UpdateGroupName(){}
  

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void UpdateGroupName::setText(const QString& text) {
  return mainText->setText(text);
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
  
QString UpdateGroupName::getGroup() const{
  return groupComboBox->currentText ();
}

} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */


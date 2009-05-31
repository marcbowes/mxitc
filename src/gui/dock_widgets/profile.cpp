

#include "profile.h"

namespace MXit
{

namespace GUI
{

namespace DockWidget
{



/****************************************************************************
**
** Author: Richard Baxter
**
** Widget constructor
**
****************************************************************************/
Profile::Profile(QWidget* parent, Theme &theme, MXit::Client& mxit) : MXitDockWidget(parent, theme), mxit(mxit)
{
  setupUi(this);
  
  profileSettings = new Widget::ProfileSettings();
  
  profileSettingsWidget->layout()->addWidget(profileSettings);
  
  connect(profileSettings, SIGNAL(fieldChanged()), this, SLOT(checkEnabledOfApplyButton()));
  
  connect(applyPushButton, SIGNAL(released()), this, SLOT(sendProfileChange()));
  
  checkEnabledOfApplyButton();
  
  setContentsEnabled(false);
}


/****************************************************************************
**
** Author: Richard Baxter
**
** Widget destructor
**
****************************************************************************/
Profile::~Profile()
{

}


void Profile::setContentsEnabled(bool enabled) {
  dockWidgetContents->setEnabled(enabled);

}


bool Profile::checkIfApplyPressable() {
  return !profileSettings->passwordLineEdit->text().isEmpty()&&!profileSettings->nicknameLineEdit->text().isEmpty();

}

void Profile::checkEnabledOfApplyButton() {
  applyPushButton->setEnabled(checkIfApplyPressable());
}


void Profile::sendProfileChange () {
  if (checkIfApplyPressable()) {
    
    /*TODO send profile change packet*/
    mxit.updateProfile(
         profileSettings->passwordLineEdit->text(), /*const QString &pin*/ 
         profileSettings->nicknameLineEdit->text(), /*const QString &name */
         profileSettings->hiddenLoginNameCheckBox->isEnabled(),/*bool hiddenLoginname*/
         profileSettings->dateEdit->date (), /*const QDate &dateOfBirth */
         QString::number(profileSettings->genderComboBox->currentIndex())); /*const QString &gender*/
  }
}


} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */





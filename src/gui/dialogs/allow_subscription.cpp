
#include "allow_subscription.h"

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
AllowSubscription::AllowSubscription(const QString & inviteMessage, const QString & nickname, const QStringList& groups, QWidget *parent) :
 QDialog(parent)
{
  setupUi(this);
  
  inviteMessageTextEdit->setText(inviteMessage);
  nicknameLineEdit->setText(nickname);
  
  groupComboBox->addItems ( groups );
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
AllowSubscription::~AllowSubscription(){}
  
  

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
  
QString AllowSubscription::getGroup() const{
  return groupComboBox->currentText ();
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

QString AllowSubscription::getNickname()  const{
  return nicknameLineEdit->text();
}

} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */


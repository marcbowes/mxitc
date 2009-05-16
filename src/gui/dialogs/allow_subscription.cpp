
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
AllowSubscription::AllowSubscription(const QString & inviteMessage, const QString & nickname, const QSet<QString>& groups, QWidget *parent) :
 QDialog(parent),  nickname(nickname), groups(groups)
{
  setupUi(this);
  
  inviteMessageTextEdit->setText(inviteMessage);
  nicknameLineEdit->setText(nickname);
  
    groupComboBox->addItem ( "" );
  Q_FOREACH(QString s, groups) {
    groupComboBox->addItem ( s );
  }
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


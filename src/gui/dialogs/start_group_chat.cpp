
#include "start_group_chat.h"

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
StartGroupChat::StartGroupChat(QWidget *parent) :
 QDialog(parent)
{
  setupUi(this);
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
StartGroupChat::~StartGroupChat(){}
  
 

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void StartGroupChat::setText(const QString& text) {
  return mainText->setText(text);
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

QString StartGroupChat::getRoomname() const {
  return roomnameLineEdit->text();
}

} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */


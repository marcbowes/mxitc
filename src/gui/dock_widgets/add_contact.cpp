

#include "add_contact.h"

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
AddContact::AddContact(QWidget* parent, Theme &theme) : MXitDockWidget(parent, theme)
{
  setupUi(this);
  
  connect(addButton, SIGNAL(released()), this, SLOT(sendAddContactInfo()));
}



/****************************************************************************
**
** Author: Richard Baxter
**
** Widget destructor
**
****************************************************************************/
AddContact::~AddContact()
{

}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void AddContact::sendAddContactInfo() {

  Protocol::Enumerables::Contact::AlertProfile a = Protocol::Enumerables::Contact::General;
  if (silentRadioButton->isChecked())
    a = Protocol::Enumerables::Contact::Silent;
  else if (meetingRadioButton->isChecked())
    a = Protocol::Enumerables::Contact::Meeting;

  emit addContact(cellphoneLineEdit->text(), nicknameLineEdit->text(), inviteMsgLineEdit->text(), a);
}


} /* end of DockWidget namespace */


} /* end of GUI namespace */

} /* end of MXit namespace */





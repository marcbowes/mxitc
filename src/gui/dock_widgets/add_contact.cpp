

#include "add_contact.h"

#include "gui/dialogs/register_gateway.h"

#include <QDebug>

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
AddContact::AddContact(QWidget* parent, Theme &theme, MXit::Client& mxit) : MXitDockWidget(parent, theme), mxit(mxit)
{
  setupUi(this);
  
  /*
  enum Type {
    MXit           = 0,
    Jabber         = 1,
    Msn            = 2,
    Yahoo          = 3,
    Icq            = 4,
    Aim            = 5,
    Bot            = 8,
    ChatRoom       = 9,
    Gallary        = 12,
    Info           = 13,
    Multimx        = 14,
    GoogleTalk     = 18
  };
  */
  alertProfileComboBox->setEnabled(false); /*TODO, do something with it, for now it does nothing*/
  
  refresh();
  
  
  /*TODO change "cellphone" lable to Address when MXit not slected as network*/
  /*TODO blank out add contact if a network is chosen*/
  
  connect(addButton, SIGNAL(released()), this, SLOT(sendAddContactInfo()));
  
  connect(networkComboBox, SIGNAL(currentIndexChanged ( int )), this, SLOT(networkChanged( int )));
  
  connect(registerButton, SIGNAL(released()), this, SLOT(registerSelectedGateway()));
  connect(unregisterButton, SIGNAL(released()), this, SLOT(unregisterSelectedGateway()));
  
  /*connect(this, 
          SIGNAL(addContact(const QString &, const QString &, const QString &, Protocol::Enumerables::Contact::Type, const QString &)), 
          mxit, 
          SLOT  (addContact(const QString &, const QString &, const QString &, Protocol::Enumerables::Contact::Type, const QString &))  );*/
  
  networkChanged(networkComboBox->currentIndex());
  setContentsEnabled(false);
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


void AddContact::setContentsEnabled(bool enabled) {
  dockWidgetContents->setEnabled(enabled);

}
/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void AddContact::registerSelectedGateway() {
  qDebug() << "wtfffff";
  Dialog::RegisterGateway reg(QString("Register to ")+networkComboBox->currentText(), false, 0);
  
  
  if (reg.exec() == QDialog::Accepted) {
    mxit.registerGateway(reg.getUsername(), reg.getPassword(),(MXit::Protocol::Enumerables::Contact::Type)networkComboBox->itemData (networkComboBox->currentIndex()).toInt());
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void AddContact::unregisterSelectedGateway() {
  Dialog::RegisterGateway reg(QString("Unregister from ")+networkComboBox->currentText(), true, 0);
  
  
  if (reg.exec() == QDialog::Accepted) {
    mxit.unregisterGateway(reg.getUsername(), (MXit::Protocol::Enumerables::Contact::Type)networkComboBox->itemData (networkComboBox->currentIndex()).toInt());
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Widget destructor
**
****************************************************************************/
void AddContact::refresh() {

  networkComboBox->clear();
  #define ADD(x) networkComboBox->addItem ( theme.contact.type.pixmap(x), #x, QVariant(x) )
  {
    using namespace Protocol::Enumerables::Contact;
    ADD(MXit);
    ADD(Jabber);
    ADD(MSN);
    ADD(Yahoo);
    ADD(ICQ);
    ADD(AIM);
    ADD(Bot);
    ADD(ChatRoom);
    ADD(Gallery);
    ADD(Info);
    ADD(MultiMx);
    ADD(GoogleTalk);
  }
  #undef ADD
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Widget destructor
**
****************************************************************************/
void AddContact::networkChanged ( int index ) {

  using namespace Protocol::Enumerables::Contact;

  Type type = (Type)networkComboBox->itemData (index).toInt();
  
  if (
        type == Jabber || 
        type == MSN || 
        type == Yahoo || 
        type == ICQ || 
        type == AIM || 
        type == GoogleTalk) {
    //contactWidget->setEnabled(false);
    gatewayWidget->setEnabled(true);
  }
  else {
    //contactWidget->setEnabled(true);
    gatewayWidget->setEnabled(false);
    if (type == MXit || type == MultiMx)
      addressLabel->setText("Cellphone");
    else
      addressLabel->setText("Address");
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void AddContact::sendAddContactInfo() {

  /*Protocol::Enumerables::Contact::AlertProfile a = Protocol::Enumerables::Contact::General;
  if (silentRadioButton->isChecked())
    a = Protocol::Enumerables::Contact::Silent;
  else if (meetingRadioButton->isChecked())
    a = Protocol::Enumerables::Contact::Meeting;*/

  //const QString &group, const QString &contactAddress, const QString &nickname, Protocol::Enumerables::Contact::Type type, const QString &message

  mxit.addContact(
                    groupComboBox->currentText (), 
                    cellphoneLineEdit->text(), 
                    nicknameLineEdit->text(), 
                    (Protocol::Enumerables::Contact::Type)networkComboBox->itemData (networkComboBox->currentIndex ()).toInt(), 
                    inviteMsgLineEdit->text()  );
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void AddContact::updateGroups(const QStringList& newGroupNames) {

  QString selectedGroup =  groupComboBox->currentText();
  
  groupComboBox->clear();
  groupComboBox->addItems ( newGroupNames );
  
  groupComboBox->setCurrentIndex  ( groupComboBox->findText(selectedGroup) );


}

} /* end of DockWidget namespace */


} /* end of GUI namespace */

} /* end of MXit namespace */





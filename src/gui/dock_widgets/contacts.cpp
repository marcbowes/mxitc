

#include "contacts.h"

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
Contacts::Contacts(const ContactMetaData * metadata, QWidget* parent) : MXitDockWidget(parent), metadata(metadata)
{
  setupUi(this);
  
  connect(contactList, SIGNAL(itemPressed ( QListWidgetItem *  )), this, SIGNAL(outgoingItemPressed( QListWidgetItem *  )));
  
}



/****************************************************************************
**
** Author: Richard Baxter
**
** Widget destructor
**
****************************************************************************/
Contacts::~Contacts()
{
  clearList();
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contacts::clearList(){

  while(contactList->count()) {
    delete contactList->takeItem (0);
  }
  
  contactList->clear();
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contacts::addContact(const Contact & c){

  QListWidgetItem * item = new QListWidgetItem(metadata->presenceIcons()[c.presence], QString().setNum(metadata->presencePrecedence()[c.presence])+c.getNickname());
  //qDebug() << c.getNickname() << ":" << c.unreadMessage;
  if (c.unreadMessage) {
    item->setForeground(QBrush(Qt::red));
  }
    
  contactList->addItem(item);
}


/****************************************************************************
**
** Author: Richard Baxter
**
**
****************************************************************************/

void Contacts::refresh(const QList<Contact>& contacts) {

  if (contacts.size() != contactList->count()) {
    /* means the new contact list has added or removed a contact*/
    /*todo, handle this case :/ */
  }
  int selected = contactList->currentRow ();
  

  //qDebug() << "refreshing";
  /* resetting contacts list*/
  clearList();/* FIXME make a tree view ?*/
  Q_FOREACH(const Contact & c, contacts) {
  
    addContact( c ); 
  }
  contactList->sortItems();
    
  for (int i = 0 ; i < contactList->count() ; i++) {
    QListWidgetItem * lwi = contactList->item(i);
    lwi->setText ( lwi->text().mid ( 1 ) );
  }
  
  contactList->setCurrentRow ( selected );

}




} /* end of DockWidget namespace */


} /* end of GUI namespace */

} /* end of MXit namespace */





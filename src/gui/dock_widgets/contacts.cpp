

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
Contacts::Contacts(QWidget* parent) : MXitDockWidget(parent)
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
** Author: Marc Bowes
**
****************************************************************************/

void Contacts::addContact(const Contact & c){


  
  QPixmap pixmap; /* FIXME: theme */
  QChar   sortPrefix;
  switch (c.presence) {
    case Protocol::Enumerables::Contact::Available:
      sortPrefix = '0';
      break;
    case Protocol::Enumerables::Contact::Online:
      sortPrefix = '1';
      break;
    case Protocol::Enumerables::Contact::Away:
      sortPrefix = '2';
      break;
    case Protocol::Enumerables::Contact::DoNotDisturb:
      sortPrefix = '3';
      break;
    case Protocol::Enumerables::Contact::Offline:
      sortPrefix = '4';
      break;
    default:
      sortPrefix = '9';
      break;
  }
  
  QListWidgetItem *item = new QListWidgetItem(pixmap, QString("%1%2").arg(sortPrefix).arg(c.nickname));
  
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





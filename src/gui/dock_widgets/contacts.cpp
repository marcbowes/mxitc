

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
Contacts::Contacts(QWidget* parent, Theme &theme) : MXitDockWidget(parent, theme)
{
  setupUi(this);
  
  connect(contactsList, SIGNAL(itemPressed ( QListWidgetItem *  )), this, SIGNAL(outgoingItemPressed( QListWidgetItem *  )));
  
  
  connect(
        contactsList, 
        SIGNAL( customContextMenuRequested ( const QPoint & ) ), 
        this, 
        SLOT( contactsListContextMenuRequest( const QPoint & ) )  );
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
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contacts::contactsListContextMenuRequest(const QPoint & pos) {

  QListWidgetItem * lwi = (contactsList->itemAt ( pos.x(), pos.y() ));
  
  if (lwi) {
    QString contactAddress = lwi->text();
    qDebug() << parent();
    emit contextMenuRequest(contactsList->mapToGlobal ( pos ), contactAddress);
  }
}


/****************************************************************************
**
** Author: Richard Baxter
** Author: Marc Bowes
**
****************************************************************************/

QListWidgetItem * Contacts::addContact(const MXit::Contact & c){

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
  
  QString label = QString("%1%2").arg(sortPrefix).arg(c.nickname);
  
  QListWidgetItem * item = NULL;
  
  if (!listItemWidgets.contains(c.nickname)) {
    item = new QListWidgetItem(theme.contact.presence.pixmap(c.presence), label); // create new item
    contactsList->addItem(item);
    listItemWidgets[c.nickname] = item;
  }
  else {
    item = listItemWidgets[c.nickname];
    *item = QListWidgetItem(theme.contact.presence.pixmap(c.presence), label); // change exiting item
  }
  
  if (c.presence == Protocol::Enumerables::Contact::Unaffiliated) {
    item->setForeground(QBrush(Qt::blue));
  }
  else {
    item->setForeground(QBrush(Qt::black));
  }
  
  
  return item;
}


/****************************************************************************
**
** Author: Richard Baxter
**
**
****************************************************************************/

void Contacts::refresh(const QList<MXit::Contact>& contacts) {


  /* getting listwidgets that are already in list*/
  QSet <QListWidgetItem*> lwiInList; // nickname
  Q_FOREACH(const Contact & c, contacts) {
    QListWidgetItem* inContacts = addContact( c ); 
    lwiInList.insert(inContacts);
  }
  
  /* removing any contacts that are no longer n the list*/
  Q_FOREACH(const Contact & c, contacts) 
  {
    if (!lwiInList.contains(listItemWidgets[c.nickname]))
    {
      contactsList->removeItemWidget(listItemWidgets[c.nickname]);
      delete listItemWidgets[c.nickname];
      listItemWidgets.remove(c.nickname);
    }
  }
  
  contactsList->sortItems();
    
  /*cleaning up the precidence hack*/
  for (int i = 0 ; i < contactsList->count() ; i++) {
    QListWidgetItem * lwi = contactsList->item(i);
    lwi->setText ( lwi->text().mid ( 1 ) );
  }

}




} /* end of DockWidget namespace */


} /* end of GUI namespace */

} /* end of MXit namespace */





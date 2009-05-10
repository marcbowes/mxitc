

#include "chat_sessions.h"

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
ChatSessions::ChatSessions(QWidget* parent, Theme &theme) : MXitDockWidget(parent, theme)
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
ChatSessions::~ChatSessions()
{
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

/*void Contacts::clearList(){

  while(contactList->count()) {
    delete contactList->takeItem (0);
  }
  
  contactList->clear();
}*/

/****************************************************************************
**
** Author: Richard Baxter
** Author: Marc Bowes
**
****************************************************************************/

QListWidgetItem * ChatSessions::addChatSession(const ChatSession & c){

  QChar   sortPrefix;
  switch (c.mainContact->presence) {
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
  
  QString label = QString("%1%2").arg(sortPrefix).arg(c.chatSessionName);
  
  QListWidgetItem * item = NULL;
  
  if (!listItemWidgets.contains(c.chatSessionName)) {
    item = new QListWidgetItem(theme.contact.presence.pixmap(c.mainContact->presence), label); // create new item
    contactList->addItem(item);
    listItemWidgets[c.chatSessionName] = item;
  }
  else {
    item = listItemWidgets[c.chatSessionName];
    *item = QListWidgetItem(theme.contact.presence.pixmap(c.mainContact->presence), label); // change exiting item
  }
    
  if (c.unreadMessage) {
    item->setForeground(QBrush(Qt::red));
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

void ChatSessions::refresh(const QList<ChatSession>& chatSessions) {


 
  /* resetting contacts list*/
  
  QSet <QListWidgetItem*> lwiInList; // nickname -> bool
  
  Q_FOREACH(const ChatSession & c, chatSessions) {
    QListWidgetItem* inContacts = addChatSession( c ); 
    lwiInList.insert(inContacts);
  }
  
  /* removing any contacts that are no longer n the list*/
  Q_FOREACH(const ChatSession & c, chatSessions) {
    if (!lwiInList.contains(listItemWidgets[c.chatSessionName])) {
      contactList->removeItemWidget(listItemWidgets[c.chatSessionName]);
      delete listItemWidgets[c.chatSessionName];
      listItemWidgets.remove(c.chatSessionName);
    }
  }
  
  /*cleaning up*/
  contactList->sortItems();
    
  for (int i = 0 ; i < contactList->count() ; i++) {
    QListWidgetItem * lwi = contactList->item(i);
    lwi->setText ( lwi->text().mid ( 1 ) );
  }
  
  //contactList->setCurrentRow ( selected );

}




} /* end of DockWidget namespace */


} /* end of GUI namespace */

} /* end of MXit namespace */





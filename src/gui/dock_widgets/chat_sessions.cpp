

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
  
  connect(chatSessionsList, SIGNAL(itemPressed ( QListWidgetItem *  )), this, SIGNAL(outgoingItemPressed( QListWidgetItem *  )));
  
  
  connect(
        chatSessionsList, 
        SIGNAL( customContextMenuRequested ( const QPoint & ) ), 
        this, 
        SLOT( chatSessionsListContextMenuRequest( const QPoint & ) )  );
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

void ChatSessions::chatSessionsListContextMenuRequest(const QPoint & pos) {

  QListWidgetItem * lwi = (chatSessionsList->itemAt ( pos.x(), pos.y() ));
  
  if (lwi) {
    QString chatSession = lwi->text();
    qDebug() << parent();
    emit contextMenuRequest(chatSessionsList->mapToGlobal ( pos ), chatSession);
  }
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


  /*FIXME TODO sort by last timestamp*/
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
    /*the chatSession is not in the list currently*/
    item = new QListWidgetItem(theme.contact.presence.pixmap(c.mainContact->presence), label); // create new item
    chatSessionsList->addItem(item);
    listItemWidgets[c.chatSessionName] = item;
  }
  else {
    /*the chatSession is in the list currently*/
    item = listItemWidgets[c.chatSessionName];
    *item = QListWidgetItem(theme.contact.presence.pixmap(c.mainContact->presence), label); // change exiting item
  }
    
  /* TODO make the dockwidget lable go red/green when there is a new message (from anyone) and it hasn't been selected*/
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
** NOTE Assusmes that chatSessionName is definitly in the list
**
****************************************************************************/

void ChatSessions::selectItem(const QString& chatSessionName) {

  
  for (int i = 0 ; i < chatSessionsList->count() ; i++) {
    
    qDebug() <<  chatSessionsList->item(i)->text();
  }
  qDebug() << chatSessionName;
  qDebug() << chatSessionsList->findItems ( chatSessionName, Qt::MatchFixedString );

  chatSessionsList->setCurrentRow ( chatSessionsList->row ( chatSessionsList->findItems ( chatSessionName, Qt::MatchFixedString ).front()));
  
  qDebug() << chatSessionsList->currentRow();
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void ChatSessions::refresh(const QList<ChatSession>& chatSessions) {
  

  /* resetting chatsessions list*/
  
  //qDebug() << "adding items";
  /*Q_FOREACH(const ChatSession & c, chatSessions) {
    qDebug() << c.chatSessionName;
  }*/
  QSet <QListWidgetItem*> shouldBeInList; // chatSessionName
  
  /* building set of who should be in the list*/
  Q_FOREACH(const ChatSession & c, chatSessions) {
    QListWidgetItem* inChatSessions = addChatSession( c ); 
    shouldBeInList.insert(inChatSessions);
    //qDebug() << "item should be in list => " << c.chatSessionName;
  }
  
  /*scanning for those who shouldn't be in the list and deleting them*/
  for (int i = 0 ; i < chatSessionsList->count() ; i++) {
    QListWidgetItem * lwi = chatSessionsList->item(i);
    
    //qDebug() << "checking if item is supposed to be in list => " << lwi->text();
    if (!shouldBeInList.contains(lwi)) {
      //qDebug() << lwi->text() << " item is in chatSessionlist but should not be";
      chatSessionsList->removeItemWidget(lwi);
      listItemWidgets.remove(lwi->text());
      delete lwi;
      i--;
    }
  }
  
  /*at this point all contacts have a precidence number on*/
  
  chatSessionsList->sortItems();
  
  /* removing precidence number*/
  for (int i = 0 ; i < chatSessionsList->count() ; i++) {
    QListWidgetItem * lwi = chatSessionsList->item(i);
    lwi->setText ( lwi->text().mid ( 1 ) );
  }
  //contactList->setCurrentRow ( selected );

}




} /* end of DockWidget namespace */


} /* end of GUI namespace */

} /* end of MXit namespace */





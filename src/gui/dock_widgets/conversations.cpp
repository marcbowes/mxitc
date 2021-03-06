/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "conversations.h"

#include <QMenu>
#include <QFileDialog>

namespace MXit
{

namespace GUI
{

namespace DockWidget
{

/****************************************************************************
        __                                _ ____    
   ____/ /__ ____ ___   ___ ___  ___ ____(_) _(_)___
  / __/ / _ `(_-<(_-<  (_-</ _ \/ -_) __/ / _/ / __/
  \__/_/\_,_/___/___/ /___/ .__/\__/\__/_/_//_/\__/ 
                         /_/                        

****************************************************************************/


/****************************************************************************
**
** Author: Marc Bowes
** Author: Richard Baxter
**
** Constructor for outgoing chat (to a Contact)
**
****************************************************************************/
Conversations::Conversations(QWidget* parent, Theme &theme, MXit::Client& mxit, MXit::Conversations & conversations) : MXitDockWidget(parent, theme), mxit(mxit), conversations(conversations)
{
  setupUi(this);
  
  
  //connect(  &conversations, SIGNAL( updated(const Conversation*)),
 //          this, SLOT (conversationUpdated(const Conversation*)));
  
  /* clicking on a conversation emits signal#conversationSelected */
  connect(conversationsList, SIGNAL(itemPressed(QListWidgetItem*)),
          this,              SLOT(emitConversationShowRequest(QListWidgetItem*)));
  
  /* right clicking on a conversation emits signal#conversationContextMenuRequest */
  connect(conversationsList, SIGNAL(customContextMenuRequested(const QPoint&)), 
          this,              SLOT(popUpContextMenu(const QPoint &)));
          
         
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Default deconstructor
**
****************************************************************************/
Conversations::~Conversations()
{
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Conversations::incomingConversationRequest            (const Conversation *conversation) {
  /*do nothing*/
  
  refresh(conversations.getConversations());
  conversationToLwi.value(conversation)->setSelected(true);
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void Conversations::incomingConversationCloseRequest       (const Conversation *conversation) {
  /*do nothing*/
  refresh(conversations.getConversations());
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void Conversations::incomingConversationShowRequest        (const Conversation *conversation) {
  /*do nothing*/
  incomingConversationRequest (conversation);
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void Conversations::incomingConversationReadNotification   (const Conversation *conversation) {

  if (conversationToLwi.contains(conversation)) {
    conversationToLwi.value(conversation)->setForeground ( QBrush(Qt::black) );
    
    refreshListWidgetItem(conversationToLwi.value(conversation));
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void Conversations::incomingConversationUpdated            (const Conversation *conversation) {

  refresh(conversations.getConversations());
  if (conversationToLwi.contains(conversation)) {
    conversationToLwi.value(conversation)->setForeground ( QBrush(Qt::red) );
    conversationToLwi.value(conversation)->setIcon ( theme.chat.unread );
    
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void Conversations::incomingConversationUpdatedFinished     (const Conversation *conversation) {

}

/****************************************************************************
                __   ___           __     __    
     ___  __ __/ /  / (_)___  ___ / /__  / /____
    / _ \/ // / _ \/ / / __/ (_-</ / _ \/ __(_-<
   / .__/\_,_/_.__/_/_/\__/ /___/_/\___/\__/___/
  /_/                                           

****************************************************************************/

  
  
/****************************************************************************
**
** Author: Richard Baxter
**
** emits a signal containing the contact selected
**
****************************************************************************/

void Conversations::refreshTheming() {
  setConversationCss();

  /*refreshing all contacts*/
  for (int i = 0 ; i < conversationsList->count() ; i++) {
    QListWidgetItem * lwi = conversationsList->item(i);
    
    refreshListWidgetItem(lwi);
  }
}

/****************************************************************************
               _           __                  __  __           __  
     ___  ____(_)  _____ _/ /____   __ _  ___ / /_/ /  ___  ___/ /__
    / _ \/ __/ / |/ / _ `/ __/ -_) /  ' \/ -_) __/ _ \/ _ \/ _  (_-<
   / .__/_/ /_/|___/\_,_/\__/\__/ /_/_/_/\__/\__/_//_/\___/\_,_/___/
  /_/                                                               

****************************************************************************/

/****************************************************************************
**
** Author: Richard Baxter
**
** refreshes a listWidgetItem (just icon for now)
**
****************************************************************************/

void Conversations::emitConversationShowRequest(QListWidgetItem *lwi) {
  if (lwi)
    emit outgoingConversationShowRequest (lwiToConversation[lwi]);
  
  //lwi->setForeground ( QBrush(Qt::black) );
}

/****************************************************************************
**
** Author: Richard Baxter
**
** refreshes a listWidgetItem (just icon for now)
**
****************************************************************************/

void Conversations::refreshListWidgetItem(QListWidgetItem *item) {
  const Conversation * conversation = lwiToConversation[item];
  
  switch (conversation->type) {
    case Protocol::Enumerables::Message::GroupChat:
      item->setIcon(theme.chat.group);
      break;
    default:
      item->setIcon(theme.contact.presence.pixmap((*conversation->getContacts().begin())->presence));
      break;
  }
  item->setText(conversation->displayName);
  
}

/****************************************************************************
                __   ___                 __  __           __  
     ___  __ __/ /  / (_)___  __ _  ___ / /_/ /  ___  ___/ /__
    / _ \/ // / _ \/ / / __/ /  ' \/ -_) __/ _ \/ _ \/ _  (_-<
   / .__/\_,_/_.__/_/_/\__/ /_/_/_/\__/\__/_//_/\___/\_,_/___/
  /_/                                                         

****************************************************************************/

/****************************************************************************
**
** Author: Marc Bowes
**
** Changes HTML head to use theme's CSS file
**
****************************************************************************/
void Conversations::setConversationCss()
{
  conversations.setCss(theme.chat.stylesheet);
}


/****************************************************************************
               _           __            __     __    
     ___  ____(_)  _____ _/ /____   ___ / /__  / /____
    / _ \/ __/ / |/ / _ `/ __/ -_) (_-</ / _ \/ __(_-<
   / .__/_/ /_/|___/\_,_/\__/\__/ /___/_/\___/\__/___/
  /_/                                                 

/***************************************************************************/

/****************************************************************************
**
** Author: Richard Baxter
**
** pops up the context menu
**
****************************************************************************/

/* trust me on this one, it's undefed after all the context menu stuffs are done - rax*/
/* FIXME overkill on the #defs? - rax*/
#define MENU_START(title) QMenu contextMenu(title, this); QHash<QString, QAction*> hash;
#define MENU_ITEM(y) hash[y] = new QAction (y, this); contextMenu.addAction(hash[y]);
#define MENU_EXEC(s) \
{ \
QAction* a = contextMenu.exec( pos ); \
bool __ret = true; \
if (a){ s = a->text(); __ret = false;} \
Q_FOREACH(QAction * act, hash){ delete act; } \
if (__ret) return;  \
}
  
void Conversations::popUpContextMenu(const QPoint &point) {
  
  QListWidgetItem * lwi = (conversationsList->itemAt ( point.x(), point.y() ));
  const Conversation * conversation;
  QPoint pos;
  
  if (lwi) {
    pos = conversationsList->mapToGlobal ( point );
    conversation = lwiToConversation[lwi];
  }
  else
    return;
    
  MENU_START(conversation->uniqueIdentifier); /* FIXME displayName rather*/
  
  //QAction title (conversation->uniqueIdentifier, this);
  //title.setEnabled(false);
  //contextMenu.addAction(&title);
  
  MENU_ITEM("Close Conversation");
  MENU_ITEM("Send File");
 
  QString selection;
  MENU_EXEC(selection);
  
  if (selection == "Close Conversation") {
    /* closes conversation */
    
    conversations.toggleActive(conversation->uniqueIdentifier);
    
    emit outgoingConversationCloseRequest ( conversation );
  }
  
  else if (selection == "Send File") {
    
    QString name = QFileDialog::getOpenFileName ();
    if (name != "") {
      ContactList contactList = ContactList::fromSet(conversation->getContacts());
      QFile file(name);
      mxit.sendFile(file, contactList);
    }
  }
}


#undef MENU_START
#undef MENU_ITEM
#undef MENU_EXEC



/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Conversations::refresh(const MXit::OrderedConversationMap& conversationsMap) {

  QSet<QListWidgetItem*> shouldBeInList;
  
  while(conversationsList->count())
    conversationsList->takeItem(0);

  /* adding/updating contacts that should be in list*/
  Q_FOREACH(Conversation* conversation, conversationsMap) {
  
    QListWidgetItem* itemToAdd;
  
    if (conversationToLwi.contains(conversation)) {
      /* then this contact already has an associated lwi*/
      /* just add it into the new list (done after since all the stuff is there)*/
      
      /* performed after */
      itemToAdd = conversationToLwi[conversation];
      //contactsList->addItem(contactAddressToLwi[contact.contactAddress]);
    }
    else {
      /* then this contact is new to the list and doesn't have an associated lwi */
      /* create a new lwi and add to hashs and then list (done after since all the stuff is there)*/
      itemToAdd = new QListWidgetItem();
      
      /* add to lookup */
      conversationToLwi[conversation] = itemToAdd;
      /* add to reverse lookup*/
      lwiToConversation[itemToAdd] = conversation;
    }
    
    /* updating listWidgetItem's lable and pixmap*/
    refreshListWidgetItem(itemToAdd);
    
    conversationsList->addItem(itemToAdd);
    if (conversation->active)
      shouldBeInList.insert(itemToAdd);
  }
  
  
  /* scanning through list items to look for ones that should not be in there */
  for (int i = 0 ; i < conversationsList->count() ; i++) {
    QListWidgetItem * lwi = conversationsList->item(i);
    
    if (shouldBeInList.contains(lwi)) {
      /* then the lwi should be in the list */
      /* do nothing */
    }
    else {
      /* then the lwi should NOT be in the list */
      /* remove it and clean up */
      
      conversationsList->removeItemWidget(lwi);
      conversationToLwi.remove(lwiToConversation[lwi]);
      lwiToConversation.remove(lwi);
      delete lwi;
      i--; /*since all the indexes above will have shifted down one and the next item will have index i now*/
      /*messing with the for loop variable (i) is bad for optimisation things... but this is a linear (slow!) parsing algorithm anyway*/
    }
  }
}


} /* end of DockWidget namespace */

}

}


/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "conversations.h"

namespace MXit
{

namespace GUI
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
Conversations::Conversations(QWidget* parent, Theme &theme)
  : MXitDockWidget(parent, theme)
{
  setupUi(this);
  
  /* clicking on a conversation emits signal#conversationSelected */
  connect(conversationsList, SIGNAL(itemPressed(QListWidgetItem*)),
          this,              SIGNAL(conversationSelected(QListWidgetItem*)));
  
  /* right clicking on a conversation emits signal#conversationContextMenuRequest */
  connect(conversationsList, SIGNAL(customContextMenuRequested(const QPoint&)), 
          this,              SLOT(conversationContextMenuRequest(const QPoint &)));
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
  Q_FOREACH(Conversation *conversation, conversations)
    delete conversation;
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
** Constructs a new Conversation from a QSet of Contacts.
**
****************************************************************************/
void Conversations::newConversation(const ContactSet &contacts)
{
  conversations.insert("", new Conversation(contacts));
  /* FIXME: add to widget list */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Constructs a new Conversation from a QSet of Contacts and assigns a
** roomName to the Conversation.
**
****************************************************************************/
void Conversations::newConversation(const ContactSet &contacts, const QString &roomName)
{
  conversations.insert(roomName, new Conversation(contacts));
  /* FIXME: add to widget list */
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
** Author: Marc Bowes
**
** emits signal#conversationContextMenuRequest if point is a valid list item
**
****************************************************************************/
void Conversations::conversationContextMenuRequest(const QPoint &point)
{
  QListWidgetItem *lwi = conversationsList->itemAt(point.x(), point.y());
  if (lwi)
    emit contextMenuRequest(lwi);
}

}

}


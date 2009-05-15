/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "conversations.h"

namespace MXit
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
**
** Constructor, using an address book to manage Contacts
**
****************************************************************************/
Conversations::Conversations(AddressBook *address_book)
  : address_book(address_book)
{
  connect(address_book, SIGNAL(updated(const ContactList&)),
          this,         SLOT(rebuild(const ContactList&)));
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
** Injects the Convesation into the internal hashes
**
****************************************************************************/
void Conversations::addConversation(Conversation *conversation)
{
  /* bubble the Conversation's update SIGNAL */
  connect(conversation, SIGNAL(updated(const Conversation*)),
          this, SIGNAL(updated(const Conversation*)));
  
  /* invert the Conversation for fast Contact-involvement lookups */
  Q_FOREACH(const Contact *contact, conversation->getContacts())
    involvements[contact].insert(conversation);
  
  /* insert into local store */
  conversations.insert(conversation->uniqueIdentifier, conversation);
  
  /* insert into a time-ordered map for sorting */
  QString orderString = conversation->lastTimestamp().toString() + conversation->uniqueIdentifier;
  orderLookup.insert(conversation->uniqueIdentifier, orderString);
  ordered.insert(orderString, conversation);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Finds a Conversation (by name and type) and appends a Message.
**
****************************************************************************/
void Conversations::addMessage(const QByteArray &contactAddress,
    const QByteArray &dateTime, const QByteArray &time,
    const QByteArray &id, const QByteArray &flags,
    const QByteArray &msg)
{
  Conversation *conversation;
  
  /* safety check */
  if (conversation = conversations.value(id)) {
    Contact *contact = address_book->contactFromAddress(contactAddress);
    /* safety check */
    if (contact)
      conversation->appendMessage(Message(*contact, msg));
  }
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
** Author: Marc Bowes
**
** SLOT which force rebuilds each Conversation which has a Contact in the
**  incoming ContactList involved.
**
****************************************************************************/
void Conversations::rebuild(const ContactList &contacts)
{
  /* find the set of Conversations to update */
  QSet<const Conversation*> toUpdate;
  Q_FOREACH(const Contact *contact, contacts)
    Q_FOREACH(const Conversation* conversation, involvements[contact])
      toUpdate.insert(conversation);
  
  /* each updated Conversation must be resorted */
  Q_FOREACH(const Conversation* conversation, toUpdate) {
    /* find the Conversation */
    OrderedConversationMap::iterator itr = ordered.find(orderLookup[conversation->uniqueIdentifier]);
    
    Conversation *update = itr.value(); /* will break for itr == .end() */
    
    /* don't even care about checking for timestamping difference (almost guarenteed) */
    /* now reorder (reinsert) if Presence has changed */
    ordered.erase(itr);
    QString orderString = conversation->lastTimestamp().toString() + conversation->uniqueIdentifier;
    orderLookup[conversation->uniqueIdentifier] = orderString;
    ordered.insert(orderString, update);
  }
}

}


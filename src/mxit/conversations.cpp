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
Conversations::Conversations(AddressBook *address_book, const QDir &log)
  : address_book(address_book), log(log)
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
  
  emit updated(conversation);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Finds a Conversation (by name and type) and appends a Message.
**
****************************************************************************/
void Conversations::addMessage(const QByteArray &contactAddress,
    const QByteArray &dateTime, const QByteArray &type,
    const QByteArray &id, const QByteArray &flags,
    const QByteArray &msg)
{
  Conversation *conversation;
  
  /* safety check */
  if (conversation = conversations.value(id)) {
    Contact *contact = address_book->contactFromAddress(contactAddress);
    /* either from a contact, or from us */
    if (contact)
      conversation->appendMessage(Message(*contact, msg, flags));
    else
      conversation->appendMessage(Message(msg, true));//FIXME: use a value from prefs
    
    /* FIXME: only write new message */
    if (log.exists()) {
      QFile writeOut(log.filePath(id + ".log.html"));
      if (writeOut.open(QFile::WriteOnly))
        writeOut.write(conversation->conversationHtml.toUtf8());
    }
  }
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Returns a pointer to a Conversation (if exists).
**
****************************************************************************/
const Conversation* Conversations::getConversation(const QString &uniqueIdentifier)
{
  return conversations.value(uniqueIdentifier);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Returns a [timestamp, name]-ordered list of Conversations
**
****************************************************************************/
const OrderedConversationMap& Conversations::getConversations()
{
  return ordered;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Returns a set of Conversation pointers for which the given Contact is
**  involved.
**
****************************************************************************/
QSet<const Conversation*> Conversations::getInvolvements(const Contact *contact)
{
  return involvements.value(contact);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Rewrites the stylesheet link for each Conversation
**
****************************************************************************/
void Conversations::setCss(const QString &location)
{
  Q_FOREACH(Conversation *conversation, conversations)
    conversation->setCss(location);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Bubbles to the Conversation#conversationHtml
**
****************************************************************************/
void Conversations::setHtml(const QString &uniqueIdentifier, const QString &html)
{
  Conversation *conversation = conversations.value(uniqueIdentifier);
  if (conversation)
    conversation->conversationHtml = html;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Bubbles to the Conversation#toggleActive
**
****************************************************************************/
void Conversations::toggleActive(const QString &uniqueIdentifier)
{
  Conversation *conversation = conversations.value(uniqueIdentifier);
  if (conversation)
    conversation->toggleActive();
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
    emit updated(conversation);
  }
}

}


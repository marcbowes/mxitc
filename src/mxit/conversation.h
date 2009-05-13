/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Conversation is essentially a join model between contacts, where
** the goal is to communicate. There can be an arbitrary number of contacts
** in this model.
**
****************************************************************************/

#ifndef __MXIT_CONVERSATION_H__
#define __MXIT_CONVERSATION_H__

#include <QList>
#include <QSet>

#include "contact.h"
#include "message.h"

namespace MXit
{

typedef QSet<const Contact*>  ContactSet;   /* for very fast contact lookups */
typedef QList<const Contact*> ContactList;  /* for synchronising with the outside world */
typedef QList<Message>         MessageList;  /* for storing chat */

class Conversation
{
  public:         /* class specific */
	
  Conversation();
  Conversation(const ContactSet &contacts);
  ~Conversation();

  public:         /* methods */
  
  void addContact(const Contact *contact);
  void addContacts(const ContactList &contacts);
  void removeContact(const Contact *contact);
  void removeContacts(const ContactList &contacts);

  private:        /* variables */
  
  ContactSet      contacts;
};

}

#endif /* __MXIT_CONVERSATION_H__ */


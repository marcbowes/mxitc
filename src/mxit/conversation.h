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

namespace MXit
{

typedef QSet<Contact*> ContactSet; /* for very fast contact lookups */

class Conversation
{
  public:         /* class specific */
	
  Conversation();
  Conversation(const ContactSet &contacts);
  ~Conversation();

  public:         /* methods */
  
  void addOrUpdateContacts(const QList<Contact*> &contacts);

  private:        /* variables */
  
  ContactSet      contacts;
};

}

#endif /* __MXIT_CONVERSATION_H__ */


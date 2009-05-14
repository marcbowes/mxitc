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

#include <QHash>

#include "contact.h"
#include "message.h"

namespace MXit
{

class Conversation : public QObject
{
  Q_OBJECT
  
  public:         /* class specific */
	
  Conversation(const Contact *contact);
  Conversation(const ContactSet &contacts, const QString &roomName="");
  ~Conversation();
  
  enum Type {
    Group,
    Private
  };
  
  signals:
  
  void updated(const Conversation*);

  public:         /* methods */
  
  void addContact(const Contact *contact);
  void addContacts(const ContactList &contacts);
  void appendMessage(const Message &message);
  const ContactSet& getContacts() const;
  void removeContact(const Contact *contact);
  void removeContacts(const ContactList &contacts);

  public:         /* variables */
  
  const QString  displayName;
  const Type     type;

  private:        /* variables */
  
  ContactSet      contacts;
  MessageList     messages;
};

typedef QHash<QString, Conversation*> ConversationHash;

}

#endif /* __MXIT_CONVERSATION_H__ */


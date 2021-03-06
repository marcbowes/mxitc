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
#include <QTime>

#include "gui/theme.h" /* FIXME: reliance on GUI code */

#include "protocol/enumerables/message.h"

#include "contact.h"
#include "message.h"

namespace MXit
{


class Conversation : public QObject
{
  Q_OBJECT
  
  public:         /* class specific */
	
  Conversation(const Contact *contact, const GUI::Theme &theme); /* FIXME: reliance on GUI code */
  Conversation(const ContactSet &contacts, const GUI::Theme &theme, const QString &roomName=""); /* FIXME: reliance on GUI code */
  ~Conversation();
  
  signals:
  
  void updated(const Conversation*);
  void messageAdded(const Conversation*);

  public:         /* methods */
  
  void addContact(const Contact *contact);
  void addContacts(const ContactList &contacts);
  void appendMessage(const Message &message);
  const ContactSet& getContacts() const;
  QTime lastTimestamp() const;
  void removeContact(const Contact *contact);
  void removeContacts(const ContactList &contacts);
  void setCss(const QString &style);
  void toggleActive();

  public:         /* variables */
  
  bool            active;
  QString         conversationHtml;
  const QString   displayName;
  MessageList     messages;
  const Protocol::Enumerables::Message::Type
                  type;
  const QString   uniqueIdentifier;
  
  private:        /* methods */
  
  static QString buildDisplayName(const ContactSet &contacts);

  private:        /* variables */
  
  ContactSet      contacts;
  const GUI::Theme &theme; /* FIXME: reliance on GUI code */
};


typedef QHash<QString, Conversation*> ConversationHash;
typedef QList<const Conversation*>   ConversationList;
typedef QSet<const Conversation*>    ConversationSet;

}

#endif /* __MXIT_CONVERSATION_H__ */


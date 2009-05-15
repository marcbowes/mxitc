/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Conversations is a container class for an arbitray number of
**  MXit::Conversation instances, providing methods to create
**
****************************************************************************/

#ifndef __MXIT_CONVERSATIONS_H__
#define __MXIT_CONVERSATIONS_H__

#include "protocol/enumerables/message.h"

#include "address_book.h"
#include "contact.h"
#include "conversation.h"

namespace MXit
{

typedef QMap<QString, Conversation*> OrderedConversationMap;

class Conversations : public QObject
{
  Q_OBJECT
  
  public:         /* class specific */

  Conversations(AddressBook *address_book);
  ~Conversations();
  
  signals:
  
  void updated(const Conversation*);
  
  public:         /* methods */
  
  void addConversation(Conversation *conversation);
  void addMessage(const QByteArray &contactAddress,
    const QByteArray &dateTime, const QByteArray &time,
    const QByteArray &id, const QByteArray &flags,
    const QByteArray &msg);
  const OrderedConversationMap& getConversations();
  
  private slots:
  
  void rebuild(const ContactList &contacts);
  
  private:        /* variables */
  
  AddressBook        *address_book;
  ConversationHash    conversations;
  QHash< const Contact*, QSet<const Conversation*> >
                      involvements;
  QHash<QString, QString>
                      orderLookup;
  OrderedConversationMap
                      ordered;
};

}

#endif /* __MXIT_CONVERSATIONS_H__ */


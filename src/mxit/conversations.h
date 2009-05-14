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
  
  const OrderedConversationMap& getConversations();
  void newGroupConversation(const ContactSet &contacts,
    const QString &roomName="");
  void newPrivateConversation(const Contact *contact);
  
  private slots:
  
  void rebuild(const ContactList &contacts);
  
  private:        /* methods */
  
  void injectNewConversation(Conversation *conversation);
  
  private:        /* variables */
  
  AddressBook const *address_book;
  QHash< const Contact*, QSet<const Conversation*> >
                      involvements;
  ConversationHash    groupConversations;
  OrderedConversationMap
                      ordered;
  ConversationHash    privateConversations;
};

}

#endif /* __MXIT_CONVERSATIONS_H__ */


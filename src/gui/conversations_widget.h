#ifndef __MXIT_GUI_CONVERSATIONS_WIDGET_H__
#define __MXIT_GUI_CONVERSATIONS_WIDGET_H__

//#include <QObject>

#include "mxit/conversation.h"
#include "mxit/conversations.h"
#include "mxit/address_book.h"

namespace MXit
{

namespace GUI
{


class ConversationsWidget /*TODO, do I need this virtual here?*/ {

  ///Q_OBJECT
  
  public: /* class specific */
  
  ConversationsWidget (Conversations& conversations, AddressBook& addressBook);
  virtual ~ConversationsWidget ();
  
  //signals:
  /*
  virtual void outgoingConversationRequest            (const Conversation *) = 0; // user requested, from this object, this conversation be shown in list
  virtual void outgoingConversationShowRequest        (const Conversation *) = 0; // user requested, from this object, this conversation be shown in UI so they may chat
  virtual void outgoingConversationCloseRequest       (const Conversation *) = 0; // user requested, from this object, this conversation be closed in list (implies it won't be shown as well)
  virtual void outgoingConversationReadNotification   (const Conversation *) = 0; // user, from this object, read the conversation
  virtual void outgoingMessage                        (QString& message, const Conversation *) = 0; // user, from this object, sent a message to a conversation

  public:

  virtual void incomingConversationRequest            (const Conversation *) = 0; // user requested, from another object, this conversation be shown in list
  virtual void incomingConversationShowRequest        (const Conversation *) = 0; // user requested, from another object, this conversation be shown in UI so they may chat
  virtual void incomingConversationCloseRequest       (const Conversation *) = 0; // user requested, from another object, this conversation be closed in list (implies it won't be shown as well)
  virtual void incomingConversationReadNotification   (const Conversation *) = 0; // user, from another object, read the conversation
  virtual void incomingConversationUpdated            (const Conversation *) = 0; // this conversation has been updated
  
  */
  protected: /* variables */
  
  Conversations& conversations;
  AddressBook& addressBook;
  
};



} /* end of GUI namespace */

} /* end of MXit namespace */



#endif /* __MXIT_GUI_CONVERSATION_TAB_WIDGE_H__ */


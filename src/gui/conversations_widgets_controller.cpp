

#include "conversations_widgets_controller.h"


namespace MXit
{

namespace GUI
{


/****************************************************************************
**
** Author: Richard Baxter
**
** Widget constructor
**
****************************************************************************/

ConversationsWidgetsController::ConversationsWidgetsController(Theme &theme, MXit::Client &mxit, Conversations& conversations, AddressBook& addressBook) : theme(theme), mxit(mxit), conversations(conversations), addressBook(addressBook)  {


  
  /*FIXME - to chat area controller*///connect(mainWebView, SIGNAL(linkClicked(const QUrl&)), mxit, SLOT(linkClicked(const QUrl&)));
  
  
}

  
/****************************************************************************
**
** Author: Richard Baxter
**
** Widget destructor
**
****************************************************************************/

ConversationsWidgetsController::~ConversationsWidgetsController() {

}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
  
void ConversationsWidgetsController::incomingConversationShowRequest (const Contact *contact) {
  
    emit incomingConversationShowRequestsToWidgets(ensureExistanceAndActivationOfConversation(contact->contactAddress));
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void ConversationsWidgetsController::addConversationsWidget(QWidget * newConversationWidget) {
  
  QWidget* newConversationObject = (QWidget*)newConversationWidget;

  conversationsWidgets.append(newConversationWidget);
  
  /*yes, I realise this will hook up incomings and outgoing to itself*/
  Q_FOREACH(QWidget * conversationWidget, conversationsWidgets) {
  
    QWidget* conversationObject = conversationWidget;
    /* conversation requests */
    connect(
              newConversationObject, 
              SIGNAL(outgoingConversationRequest(const Conversation *)),
              conversationObject,
              SLOT(incomingConversationRequest(const Conversation *))
              );
    connect(
              conversationObject, 
              SIGNAL(outgoingConversationRequest(const Conversation *)),
              newConversationObject,
              SLOT(incomingConversationRequest(const Conversation *))
              );
              
    /* conversation show requests */
    connect(
              newConversationObject, 
              SIGNAL(outgoingConversationShowRequest(const Conversation *)),
              conversationObject,
              SLOT(incomingConversationShowRequest(const Conversation *))
              );
    connect(
              conversationObject, 
              SIGNAL(outgoingConversationRequest(const Conversation *)),
              newConversationObject,
              SLOT(incomingConversationRequest(const Conversation *))
              );
              
    /* conversation close requests */
    connect(
              newConversationObject, 
              SIGNAL(outgoingConversationCloseRequest(const Conversation *)),
              conversationObject,
              SLOT(incomingConversationCloseRequest(const Conversation *))
              );
    connect(
              conversationObject, 
              SIGNAL(outgoingConversationCloseRequest(const Conversation *)),
              newConversationObject,
              SLOT(incomingConversationCloseRequest(const Conversation *))
              );
              
    /* conversation read notification */
    connect(
              newConversationObject, 
              SIGNAL(outgoingConversationReadNotification(const Conversation *)),
              conversationObject,
              SLOT(incomingConversationReadNotification(const Conversation *))
              );
    connect(
              conversationObject, 
              SIGNAL(outgoingConversationReadNotification(const Conversation *)),
              newConversationObject,
              SLOT(incomingConversationReadNotification(const Conversation *))
              );
  
  }
          
  /* conversation outgoing message  */    
  connect(
            newConversationObject, 
            SIGNAL(outgoingMessage(QString&, const Conversation *)),
            this,
            SLOT(incomingMessageFromWidget(QString&, const Conversation *))
            );
  
  connect(
            &conversations,
            SIGNAL(updated(const Conversation *)),
            newConversationObject, 
            SLOT(incomingConversationUpdated( const Conversation *))
            );
            
  /*connect(
            this,
            SIGNAL(incomingConversationUpdatesToWidgets(const Conversation *)),
            newConversationObject, 
            SLOT(incomingConversationUpdated( const Conversation *))
            );*/
  
  connect(
            this,
            SIGNAL(incomingConversationShowRequestsToWidgets(const Conversation *)),
            newConversationObject, 
            SLOT(incomingConversationShowRequest( const Conversation *))
            );
  
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

const Conversation * ConversationsWidgetsController::ensureExistanceAndActivationOfConversation(const QString& uniqueId) {
  
  const Conversation * conversation = conversations.getConversation(uniqueId);

  if (!conversation) {
    /* conversations does not exist, need to create it*/
    /* create personal (single contact) conversation */
    Conversation *newConversation = new Conversation(addressBook.contactFromAddress(uniqueId));
    newConversation->setCss(theme.chat.stylesheet);
    conversations.addConversation(newConversation);
    
    /*this *will* return a valid pointer*/
    conversation = conversations.getConversation(uniqueId);
  }
  else {
    if (!conversation->active)
      conversations.toggleActive(conversation->uniqueIdentifier);
  }
  
  return conversation;
  
}


/****************************************************************************
**
** Author: Richard Baxter
**
** Handles an incoming message from an object (sends it to the network controller)
**
****************************************************************************/

void ConversationsWidgetsController::incomingMessageFromWidget(QString& message, const Conversation * conversation) {
  
 
  Q_FOREACH(const Contact* contact, conversation->getContacts()) {
    mxit.sendMessage(contact->contactAddress, message, Protocol::Enumerables::Message::Normal /* FIXME? */, Protocol::Enumerables::Message::MayContainMarkup);
    
    
    conversations.addMessage(   QByteArray(), /*FIXME, should be 'me'*/
                                mxit.variableValue("dateTime"),  /*FIXME, where do i get this from?*/
                                mxit.variableValue("time"), /*FIXME, where do i get this from?*/
                                QByteArray().append (contact->contactAddress),
                                mxit.variableValue("flags"), /*FIXME, where do i get this from?*/
                                QByteArray().append (message) );
  }
}


} /* end of GUI namespace */

} /* end of MXit namespace */










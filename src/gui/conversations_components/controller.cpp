

#include "controller.h"

#include <QUrl>
#include <QWebView>
#include <QWebFrame>


namespace MXit
{

namespace GUI
{

namespace ConversationsComponent
{


/****************************************************************************
**
** Author: Richard Baxter
**
** Widget constructor
**
****************************************************************************/

Controller::Controller(Theme &theme, MXit::Client &mxit, Conversations& conversations, AddressBook& addressBook) : theme(theme), mxit(mxit), conversations(conversations), addressBook(addressBook)  {
  
  
}

  
/****************************************************************************
**
** Author: Richard Baxter
**
** Widget destructor
**
****************************************************************************/

Controller::~Controller() {

}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
  
void Controller::incomingConversationShowRequest (const Contact *contact) {
  
    emit incomingConversationShowRequestsToWidgets(ensureExistanceAndActivationOfConversation(contact->contactAddress));
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Controller::addConversationsWidget(QWidget * newConversationWidget) {
  
  QWidget* newConversationObject = (QWidget*)newConversationWidget;


          
  /* conversation outgoing message  */    
            
  connect(
            newConversationObject, 
            SIGNAL(outgoingMessage(QString&, const Conversation *)),
            this,
            SLOT(incomingMessageFromWidget(QString&, const Conversation *))
            );
            
  
  connect(
            this,
            SIGNAL(incomingConversationShowRequestsToWidgets(const Conversation *)),
            newConversationObject, 
            SLOT(incomingConversationShowRequest( const Conversation *))
            );

  Q_FOREACH(QWidget * conversationWidget, conversationsWidgets) {
      
    disconnect(
            &conversations,
            SIGNAL(messageAdded(const Conversation *)),
            conversationWidget, 
            SLOT(incomingConversationUpdated( const Conversation *))
            );
      
    disconnect(
            &conversations,
            SIGNAL(messageAdded(const Conversation *)),
            conversationWidget, 
            SLOT(incomingConversationUpdatedFinished( const Conversation *))
            );
  }
  
  Q_FOREACH(QWidget * conversationWidget, conversationsWidgets) {
  
    QWidget* conversationObject = conversationWidget;
    /* conversation requests */
    connect(
              conversationObject, 
              SIGNAL(outgoingConversationRequest(const Conversation *)),
              newConversationObject,
              SLOT(incomingConversationRequest(const Conversation *))
              );
              
    /* conversation show requests */
    connect(
              conversationObject, 
              SIGNAL(outgoingConversationShowRequest(const Conversation *)),
              newConversationObject,
              SLOT(incomingConversationShowRequest(const Conversation *))
              );
              
    /* conversation close requests */
    connect(
              conversationObject, 
              SIGNAL(outgoingConversationCloseRequest(const Conversation *)),
              newConversationObject,
              SLOT(incomingConversationCloseRequest(const Conversation *))
              );
              
    /* conversation read notification */
    connect(
              conversationObject, 
              SIGNAL(outgoingConversationReadNotification(const Conversation *)),
              newConversationObject,
              SLOT(incomingConversationReadNotification(const Conversation *))
              );
  
  }
  
  conversationsWidgets.append(newConversationWidget);
  
  
  Q_FOREACH(QWidget * conversationWidget, conversationsWidgets) {
  
    QWidget* conversationObject = conversationWidget;
    /* conversation requests */
    connect(
              newConversationObject, 
              SIGNAL(outgoingConversationRequest(const Conversation *)),
              conversationObject,
              SLOT(incomingConversationRequest(const Conversation *))
              );
              
    /* conversation show requests */
    connect(
              newConversationObject, 
              SIGNAL(outgoingConversationShowRequest(const Conversation *)),
              conversationObject,
              SLOT(incomingConversationShowRequest(const Conversation *))
              );
              
    /* conversation close requests */
    connect(
              newConversationObject, 
              SIGNAL(outgoingConversationCloseRequest(const Conversation *)),
              conversationObject,
              SLOT(incomingConversationCloseRequest(const Conversation *))
              );
              
    /* conversation read notification */
    connect(
              newConversationObject, 
              SIGNAL(outgoingConversationReadNotification(const Conversation *)),
              conversationObject,
              SLOT(incomingConversationReadNotification(const Conversation *))
              );
  
  }
  
  
  
  Q_FOREACH(QWidget * conversationWidget, conversationsWidgets) {
      
    connect(
            &conversations,
            SIGNAL(messageAdded(const Conversation *)),
            conversationWidget, 
            SLOT(incomingConversationUpdated( const Conversation *))
            );
  }
  Q_FOREACH(QWidget * conversationWidget, conversationsWidgets) {
      
    connect(
            &conversations,
            SIGNAL(messageAdded(const Conversation *)),
            conversationWidget, 
            SLOT(incomingConversationUpdatedFinished( const Conversation *))
            );
  }
  
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

const Conversation * Controller::ensureExistanceAndActivationOfConversation(const QString& uniqueId) {
  
  const Conversation * conversation = conversations.getConversation(uniqueId);

  if (!conversation) {
    /* conversations does not exist, need to create it*/
    /* create personal (single contact) conversation */
    Conversation *newConversation = new Conversation(addressBook.contactFromAddress(uniqueId), theme); /* FIXME: theme passing */
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

void Controller::incomingMessageFromWidget(QString& message, const Conversation * conversation) {
  
 
  Q_FOREACH(const Contact* contact, conversation->getContacts()) {
    mxit.sendMessage(contact->contactAddress, message, Protocol::Enumerables::Message::Normal /* FIXME? */, Protocol::Enumerables::Message::MayContainMarkup);
    
    
    conversations.addMessage(   QByteArray(),
                                mxit.variableValue("dateTime"),
                                mxit.variableValue("time"),
                                QByteArray().append (contact->contactAddress),
                                mxit.variableValue("flags"),
                                QByteArray().append (message) );
  }
}


} /* end of ConversationsComponent namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */










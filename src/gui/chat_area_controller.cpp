

#include "chat_area_controller.h"


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

ChatAreaController::ChatAreaController(Theme &theme, MXit::Client &mxit, Conversations& conversations, AddressBook& addressBook) : theme(theme), mxit(mxit), conversations(conversations), addressBook(addressBook)  {


  chatTabWidget = new QTabWidget();
  chatTabWidget->setMovable ( true );
  chatTabWidget->setTabsClosable ( true );
  
  connect(chatTabWidget, SIGNAL(currentChanged( int )), this, SLOT(emitConversationChanged ( int )));
  
  connect(chatTabWidget, SIGNAL(tabCloseRequested( int )), this, SLOT(removeAndDeleteConversationFromGUI ( int )));
  
    /*FIXME - to chat area controller*///connect(mainWebView, SIGNAL(linkClicked(const QUrl&)), mxit, SLOT(linkClicked(const QUrl&)));
  
  /*TODO put this somewhere useful*/
  /*FIXME - to chat area controller*///mainWebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
  
}

  
/****************************************************************************
**
** Author: Richard Baxter
**
** Widget destructor
**
****************************************************************************/

ChatAreaController::~ChatAreaController() {
  delete chatTabWidget;

  Q_FOREACH(ChatArea * chatArea, conversationToChatArea.values()) {
    delete chatArea;
  }

}

void ChatAreaController::emitConversationChanged ( int index ) {
  emit conversationChanged ( chatAreaToConversation[chatTabWidget->widget ( index )] );
}


//void ChatAreaController::tabCloseRequested ( int index ) {

  /*TODO call or hook to the (non-existant) clean up and remove from gui and conversations method*/

//}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void ChatAreaController::updateTabOf(const  Conversation* conversation) {
  if (!conversation->active)
    return;
    
    
  
  ChatArea * chatArea = ensureExistanceOfChatAreaAndTab(conversation->uniqueIdentifier);
  //chatArea = conversationToChatArea[conversation];
  if (chatArea == NULL) {
    qDebug() << "Chat Area is NULL!! - must put the ensureConversationExists method in MXit::Conversations";
    return;
  }
  
  chatArea->chatWebView->setHtml(chatAreaToConversation[chatArea]->conversationHtml);

  QWebFrame * frame = chatArea->chatWebView->page ()->currentFrame ();
  frame->setScrollBarValue(Qt::Vertical, frame->scrollBarMaximum(Qt::Vertical));
  
  /* FIXME implement red label thing*/
  //conversationsWidget->conversationRead(currentConversation);
  /* make label red if not selected tab*/
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void ChatAreaController::sendMessageFromChatArea (const ChatArea * chatArea) {
  
  outgoingMessage(chatArea->chatInput->text(), chatAreaToConversation[chatArea]);
  
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Handles an outgoing message (sends it to the network controller)
**
****************************************************************************/

void ChatAreaController::outgoingMessage(const QString & message, const Conversation * conversation)
{
  
  //currentConversation->incomingMessage( Message ( message) );
  //currentConversation->unreadMessage = false;
  
  Q_FOREACH(const Contact* contact, conversation->getContacts()) {
    mxit.sendMessage(contact->contactAddress, message, Protocol::Enumerables::Message::Normal /* FIXME? */, Protocol::Enumerables::Message::MayContainMarkup);
    
    
    conversations.addMessage(   QByteArray(), /*FIXME, should be 'me'*/
                                mxit.variableValue("dateTime"),  /*FIXME, where do i get this from?*/
                                mxit.variableValue("time"), /*FIXME, where do i get this from?*/
                                QByteArray().append (contact->contactAddress),
                                mxit.variableValue("flags"), /*FIXME, where do i get this from?*/
                                QByteArray().append (message) );
  }
  
  //refreshChatBox();
  //chatAreaController->refreshTabs(); /*TODO check how things refresh now, might be a signal from the conversations that can be hooked up*/
}


/****************************************************************************
**
** Author: Richard Baxter
**
** returns the conversation if it exists, otehrwise creates it and returns that
**
****************************************************************************/

const Conversation * ChatAreaController::ensureExistanceOfConversation(const QString & uniqueId) {
  
  const Conversation * conversation = conversations.getConversation(uniqueId);

  if (!conversation) {
    /* conversations does not exist, need to create it*/
    /* create personal (single contact) conversation */
    Conversation *newConversation = new Conversation(addressBook.contactFromAddress(uniqueId));
    newConversation->setCss(theme.chat.stylesheet);
    conversations.addConversation(newConversation); /*FIXME causes a signal to updateTab before tab is created*/
    
    /*this *will* return a valid pointer*/
    conversation = conversations.getConversation(uniqueId);
  }
  else {
    /* conversations does exist, need to ensure it is active*/
    if (!conversation->active)
      conversations.toggleActive(uniqueId);
  }
  
  return conversation;
}

/***************************************************************************/

/*const Conversation * ChatAreaController::ensureExistanceOfConversation(const Contact * contact) {
  return ensureExistanceOfConversation(contact->contactAddress);
}*/

/***************************************************************************/

/*const Conversation * ChatAreaController::ensureExistanceOfConversation(const Conversation * conversation) {
  return ensureExistanceOfConversation(conversation->uniqueIdentifier);
}*/



/****************************************************************************
**
** Author: Richard Baxter
**
** switches to conversation (creates the tab if it does not exist already)
**
****************************************************************************/


void ChatAreaController::switchToConversationTab(const Contact * contact) {
  if (contact)
    switchToConversationTab(contact->contactAddress);
}

/***************************************************************************/

void ChatAreaController::switchToConversationTab(const Conversation * conversation) {
  if (conversation)
    switchToConversationTab(conversation->uniqueIdentifier);
}

/***************************************************************************/

void ChatAreaController::switchToConversationTab(const QString & uniqueId){
  ensureExistanceOfConversation(uniqueId);
  ensureExistanceOfChatAreaAndTab(uniqueId);
  chatTabWidget->setCurrentIndex(chatTabWidget->indexOf(conversationToChatArea[conversations.getConversation(uniqueId)]));
}




/****************************************************************************
**
** Author: Richard Baxter
** NOTE:  Assumes that the uniqueId is correct! 
**        error checking should be done higher up
**
****************************************************************************/
  
ChatArea* ChatAreaController::ensureExistanceOfChatAreaAndTab(const QString & uniqueId) {
  
  const Conversation *conversation = ensureExistanceOfConversation(uniqueId);

  if (!conversationToChatArea.contains(conversation)) {
    conversationToChatArea[conversation] = new ChatArea();
    
    ChatArea* chatArea = conversationToChatArea[conversation];
    chatAreaToConversation[chatArea] = conversation;
    chatTabWidget->addTab(chatArea, QIcon(QPixmap(16,16)), conversation->displayName);
    
    connect(
              chatArea->chatWebView, 
              SIGNAL(linkClicked(const QUrl&)), 
              &mxit, 
              SLOT(linkClicked(const QUrl&)));
    
    connect(conversationToChatArea[conversation], SIGNAL(sendMessageFromChatInput(const ChatArea *)), this, SLOT(sendMessageFromChatArea (const ChatArea *)));
  }
  
  return conversationToChatArea[conversation];
}
/*TODO, remove tab and chatarea function*/

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

QWidget * ChatAreaController::getCentralChatArea() {
  return chatTabWidget;

}



/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void ChatAreaController::removeAndDeleteConversationFromGUI( int index ){
  removeAndDeleteConversationFromGUI(chatAreaToConversation[chatTabWidget->widget(index)]);
}
  
  
/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void ChatAreaController::removeAndDeleteConversationFromGUI( const Conversation* conversation ) {
  
  if (conversationToChatArea.contains(conversation)) {
    disconnect(
                conversationToChatArea[conversation]->chatWebView, 
                SIGNAL(linkClicked(const QUrl&)), 
                &mxit, 
                SLOT(linkClicked(const QUrl&)));
                
    
    chatTabWidget->removeTab ( chatTabWidget->indexOf(conversationToChatArea[conversation]) );

    chatAreaToConversation.remove(conversationToChatArea[conversation]);
    conversationToChatArea.remove(conversation);
    
    if (conversation->active)
      conversations.toggleActive(conversation->uniqueIdentifier);
    //emit conversationRequest(NULL);
  }
}



} /* end of GUI namespace */

} /* end of MXit namespace */










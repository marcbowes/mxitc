


#include "conversations_tab_widget.h"

#include <QDebug>
#include <QTextDocument>
#include <QWebFrame>


namespace MXit
{

namespace GUI
{



/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

ConversationsTabWidget::ConversationsTabWidget(Theme &theme, MXit::Client &mxit, Conversations& conversations, AddressBook& addressBook, QWidget* parent) : QTabWidget(parent), ConversationsWidget(conversations, addressBook), mxit(mxit), nonUserTabChange(false)  {
  
  setMovable ( true );
  setTabsClosable ( true );
  
  /*FIXME, rather override the methods*/
  connect(this, SIGNAL(currentChanged( int )), this, SLOT(currentChanged ( int )));
  
  connect(this, SIGNAL(tabCloseRequested( int )), this, SLOT(tabCloseRequested ( int )));

}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

ConversationsTabWidget::~ConversationsTabWidget() {

  Q_FOREACH(ChatArea * chatArea, conversationToChatArea.values()) {
    delete chatArea;
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/


void ConversationsTabWidget::incomingConversationRequest           (const Conversation *conversation) {
  ensureExistanceOfChatAreaAndTab( conversation );
  
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void ConversationsTabWidget::incomingConversationShowRequest       (const Conversation *conversation) {
  switchToConversationTab(conversation);
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void ConversationsTabWidget::incomingConversationCloseRequest      (const Conversation *conversation) {
  removeAndDeleteConversationFromGUI( conversation );
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void ConversationsTabWidget::incomingConversationReadNotification  (const Conversation *conversation) {
  /*TODO change icon*/
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void ConversationsTabWidget::incomingConversationUpdated           (const Conversation *conversation) {

  ensureExistanceOfChatAreaAndTab( conversation );
  updateTabOf(conversation);
  
  /*TODO change icon*/
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Widget destructor
**
****************************************************************************/


void ConversationsTabWidget::currentChanged ( int index ) {
  if (!nonUserTabChange) {
    emit outgoingConversationRequest ( chatAreaToConversation[widget ( index )] );
    emit outgoingConversationReadNotification( chatAreaToConversation[widget ( index )] );
  }
}


void ConversationsTabWidget::tabCloseRequested ( int index ) {
  emit outgoingConversationCloseRequest ( chatAreaToConversation[widget ( index )] );
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void ConversationsTabWidget::updateTabOf(const  Conversation* conversation) {
  if (!conversation->active)
    return;
    
  ChatArea * chatArea = ensureExistanceOfChatAreaAndTab(conversation);
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

void ConversationsTabWidget::sendMessageFromChatArea (const ChatArea * chatArea) {
  
  QString message = chatArea->chatInput->text();
  emit outgoingMessage(message, chatAreaToConversation[chatArea]);
  
}



/****************************************************************************
**
** Author: Richard Baxter
**
** switches to conversation (creates the tab if it does not exist already)
**
****************************************************************************/


void ConversationsTabWidget::switchToConversationTab(const Conversation * conversation) {
  if (conversation) {
    ensureExistanceOfChatAreaAndTab(conversation);
    setCurrentIndex(indexOf(conversationToChatArea[conversation]));
    emit outgoingConversationReadNotification( conversation );
  }
}


/****************************************************************************
**
** Author: Richard Baxter
** NOTE:  Assumes that the uniqueId is correct! 
**        error checking should be done higher up
**
****************************************************************************/
  
ChatArea* ConversationsTabWidget::ensureExistanceOfChatAreaAndTab(const Conversation *conversation) {
  
  //const Conversation *conversation = ensureExistanceOfConversation(uniqueId);

  if (!conversationToChatArea.contains(conversation)) {
    conversationToChatArea[conversation] = new ChatArea();
    
    ChatArea* chatArea = conversationToChatArea[conversation];
    chatAreaToConversation[chatArea] = conversation;
    addTab(chatArea, QIcon(QPixmap(16,16)), conversation->displayName);
    
    connect(
              chatArea->chatWebView, 
              SIGNAL(linkClicked(const QUrl&)), 
              &mxit, 
              SLOT(linkClicked(const QUrl&)));
    
    connect(conversationToChatArea[conversation], SIGNAL(sendMessageFromChatInput(const ChatArea *)), this, SLOT(sendMessageFromChatArea (const ChatArea *)));
  }
  
  return conversationToChatArea[conversation];
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void ConversationsTabWidget::removeAndDeleteConversationFromGUI( const Conversation* conversation ) {
  
  if (conversationToChatArea.contains(conversation)) {
    disconnect(
                conversationToChatArea[conversation]->chatWebView, 
                SIGNAL(linkClicked(const QUrl&)), 
                &mxit, 
                SLOT(linkClicked(const QUrl&)));
                
    removeTab ( indexOf(conversationToChatArea[conversation]) );

    chatAreaToConversation.remove(conversationToChatArea[conversation]);
    conversationToChatArea.remove(conversation);
    
    if (conversation->active)
      conversations.toggleActive(conversation->uniqueIdentifier);
    //emit conversationRequest(NULL);
  }
}


void ConversationsTabWidget::removeTab ( int index ) {
  nonUserTabChange = true;
  QTabWidget::removeTab ( index );
  nonUserTabChange = false;
}


} /* end of GUI namespace */

} /* end of MXit namespace */






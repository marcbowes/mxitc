


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

ConversationsTabWidget::ConversationsTabWidget(Theme &theme, MXit::Client &mxit, Conversations& conversations, AddressBook& addressBook, QWidget* parent) : QTabWidget(parent), conversations(conversations), addressBook(addressBook), theme(theme), mxit(mxit), nonUserTabChange(false)  {
  
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
  disconnect();
  Q_FOREACH(ChatArea * chatArea, conversationToChatArea.values()) {
    removeTab ( indexOf(chatArea) );
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

  ChatArea * chatArea =  conversationToChatArea.value( conversation );
  /* TODO - change to group chat icon if group chat*/
  setTabIcon(indexOf(chatArea), theme.contact.presence.pixmap((*conversation->getContacts().begin())->presence));
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void ConversationsTabWidget::incomingConversationUpdated           (const Conversation *conversation) {

  if(conversation->active) {
    ChatArea * chatArea =  ensureExistanceOfChatAreaAndTab( conversation );
    updateTabOf(chatArea);
    setTabIcon ( indexOf(chatArea), theme.chat.unread );
  }
  
  
  
}
/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void ConversationsTabWidget::incomingConversationUpdatedFinished    (const Conversation *conversation) {
  
  if (currentWidget ())
    emit outgoingConversationReadNotification( chatAreaToConversation[currentWidget ()] );
  
}

/****************************************************************************
**
** Author: Richard Baxter
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


void ConversationsTabWidget::refreshThemeing() {
  
  for (int i = 0 ; i < tabBar()->count(); i++) {
    
    setTabIcon(i, theme.contact.presence.pixmap((*chatAreaToConversation[widget(i)]->getContacts().begin())->presence));
    
    updateTabOf((ChatArea*)widget(i));
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void ConversationsTabWidget::updateTabOf(ChatArea * chatArea) {

  
  chatArea->chatWebView->setHtml(chatAreaToConversation[chatArea]->conversationHtml);

  QWebFrame * frame = chatArea->chatWebView->page ()->currentFrame ();
  frame->setScrollBarValue(Qt::Vertical, frame->scrollBarMaximum(Qt::Vertical));
  
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
    int newIndex = indexOf(conversationToChatArea[conversation]);
    if (newIndex != currentIndex()) {
      setCurrentIndex(newIndex);
    }
    else {  
      emit outgoingConversationReadNotification( conversation );
    }
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
    
    
    nonUserTabChange = true;
    addTab(chatArea, QIcon(QPixmap(16,16)), conversation->displayName);
    nonUserTabChange = false;
    
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
                
    nonUserTabChange = true;
    removeTab ( indexOf(conversationToChatArea[conversation]) );
    nonUserTabChange = false;

    chatAreaToConversation.remove(conversationToChatArea[conversation]);
    conversationToChatArea.remove(conversation);
    
    if (conversation->active)
      conversations.toggleActive(conversation->uniqueIdentifier);
    //emit conversationRequest(NULL);
  }
}



} /* end of GUI namespace */

} /* end of MXit namespace */






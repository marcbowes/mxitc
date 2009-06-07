
#include "tab_widget.h"

#include <QTextDocument>
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
****************************************************************************/

TabWidget::TabWidget(Theme &theme, MXit::Client &mxit, Conversations& conversations, AddressBook& addressBook, QWidget* parent) : QTabWidget(parent), conversations(conversations), addressBook(addressBook), theme(theme), mxit(mxit), nonUserTabChange(false)  {
  
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

TabWidget::~TabWidget() {
  disconnect();
  Q_FOREACH(Widget::Conversation * conversationWidget, conversationToConversationWidget.values()) {
    removeTab ( indexOf(conversationWidget) );
    delete conversationWidget;
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/


void TabWidget::incomingConversationRequest           (const Conversation *conversation) {
  ensureExistanceOfConversationWidgetAndTab( conversation );
  
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void TabWidget::incomingConversationShowRequest       (const Conversation *conversation) {
  switchToConversationTab(conversation);
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void TabWidget::incomingConversationCloseRequest      (const Conversation *conversation) {
  removeAndDeleteConversationFromGUI( conversation );
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void TabWidget::incomingConversationReadNotification  (const Conversation *conversation) {

  Widget::Conversation * conversationWidget =  conversationToConversationWidget.value( conversation );
  /* TODO - change to group chat icon if group chat*/
  setTabIcon(indexOf(conversationWidget), theme.contact.presence.pixmap((*conversation->getContacts().begin())->presence));
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void TabWidget::incomingConversationUpdated           (const Conversation *conversation) {

  if(conversation->active) {
    Widget::Conversation * conversationWidget =  ensureExistanceOfConversationWidgetAndTab( conversation );
    updateTabOf(conversationWidget);
    setTabIcon ( indexOf(conversationWidget), theme.chat.unread );
  }
  
  
  
}
/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void TabWidget::incomingConversationUpdatedFinished    (const Conversation *conversation) {
  
  if (currentWidget ())
    emit outgoingConversationReadNotification( conversationWidgetToConversation[currentWidget ()] );
  
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void TabWidget::currentChanged ( int index ) {
  if (!nonUserTabChange) {
    emit outgoingConversationRequest ( conversationWidgetToConversation[widget ( index )] );
    emit outgoingConversationReadNotification( conversationWidgetToConversation[widget ( index )] );
  }
}


void TabWidget::tabCloseRequested ( int index ) {
  emit outgoingConversationCloseRequest ( conversationWidgetToConversation[widget ( index )] );
}


void TabWidget::refreshTheming() {
  
  for (int i = 0 ; i < tabBar()->count(); i++) {
    
    setTabIcon(i, theme.contact.presence.pixmap((*conversationWidgetToConversation[widget(i)]->getContacts().begin())->presence));
    
    updateTabOf((Widget::Conversation*)widget(i));
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void TabWidget::updateTabOf(Widget::Conversation * conversationWidget) {

  
  conversationWidget->chatWebView->setHtml(conversationWidgetToConversation[conversationWidget]->conversationHtml);

  QWebFrame * frame = conversationWidget->chatWebView->page ()->currentFrame ();
  frame->setScrollBarValue(Qt::Vertical, frame->scrollBarMaximum(Qt::Vertical));
  
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void TabWidget::sendMessageFromConversationWidget (const Widget::Conversation * conversationWidget) {
  
  QString message = conversationWidget->chatInput->text();
  emit outgoingMessage(message, conversationWidgetToConversation[conversationWidget]);
  
}



/****************************************************************************
**
** Author: Richard Baxter
**
** switches to conversation (creates the tab if it does not exist already)
**
****************************************************************************/


void TabWidget::switchToConversationTab(const Conversation * conversation) {
  if (conversation) { /*FIXME test whether this if statement needs to be here*/
    ensureExistanceOfConversationWidgetAndTab(conversation);
    int newIndex = indexOf(conversationToConversationWidget[conversation]);
    if (newIndex != currentIndex()) {
      setCurrentIndex(newIndex);
    }
    else {  
      emit outgoingConversationReadNotification( conversation );
    }
    conversationToConversationWidget[conversation]->chatInput->setFocus (Qt::OtherFocusReason);
  }
}


/****************************************************************************
**
** Author: Richard Baxter
** NOTE:  Assumes that the uniqueId is correct! 
**        error checking should be done higher up
**
****************************************************************************/
  
Widget::Conversation* TabWidget::ensureExistanceOfConversationWidgetAndTab(const Conversation *conversation) {
  
  //const Conversation *conversation = ensureExistanceOfConversation(uniqueId);

  if (!conversationToConversationWidget.contains(conversation)) {
    conversationToConversationWidget[conversation] = new Widget::Conversation();
    
    Widget::Conversation* conversationWidget = conversationToConversationWidget[conversation];
    conversationWidgetToConversation[conversationWidget] = conversation;
    
    
    nonUserTabChange = true;
    addTab(conversationWidget, QIcon(QPixmap(16,16)), conversation->displayName);
    nonUserTabChange = false;
    
    connect(
              conversationWidget->chatWebView, 
              SIGNAL(linkClicked(const QUrl&)), 
              &mxit, 
              SLOT(linkClicked(const QUrl&)));
    
    connect(conversationToConversationWidget[conversation], SIGNAL(sendMessageFromConversationWidget(const Widget::Conversation *)), this, SLOT(sendMessageFromConversationWidget (const Widget::Conversation *)));
  }
  
  return conversationToConversationWidget[conversation];
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void TabWidget::removeAndDeleteConversationFromGUI( const Conversation* conversation ) {
  
  if (conversationToConversationWidget.contains(conversation)) {
    disconnect(
                conversationToConversationWidget[conversation]->chatWebView, 
                SIGNAL(linkClicked(const QUrl&)), 
                &mxit, 
                SLOT(linkClicked(const QUrl&)));
                
    nonUserTabChange = true;
    removeTab ( indexOf(conversationToConversationWidget[conversation]) );
    nonUserTabChange = false;

    conversationWidgetToConversation.remove(conversationToConversationWidget[conversation]);
    conversationToConversationWidget.remove(conversation);
    
    if (conversation->active)
      conversations.toggleActive(conversation->uniqueIdentifier);
    //emit conversationRequest(NULL);
  }
}


} /* end of ConversationsComponent namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */






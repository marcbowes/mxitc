

#include "conversation.h"
#include <QWebFrame>


namespace MXit
{

namespace GUI
{

namespace Widget
{


/****************************************************************************
**
** Author: Richard Baxter
**
** Widget constructor
**
****************************************************************************/

Conversation::Conversation(QWidget * parent ) : QWidget(parent) {
  setupUi(this);
  
  chatWebView->setFocusProxy(chatInput);
  
  connect(chatInput,   SIGNAL(returnPressed ()), this, SLOT(emitSendMessageFromConversationWidget()));
  connect(chatInput,   SIGNAL(returnPressed ()), chatInput, SLOT(clear()));
  connect(chatWebView, SIGNAL(loadFinished ( bool )), this, SLOT(loadFinished ( bool )));
  
  chatWebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

}


  
/****************************************************************************
**
** Author: Richard Baxter
**
** Widget destructor
**
****************************************************************************/

Conversation::~Conversation() {

}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Conversation::loadFinished ( bool ok ) {
  
  QWebFrame * frame = chatWebView->page ()->currentFrame ();
  frame->setScrollBarValue(Qt::Vertical, frame->scrollBarMaximum(Qt::Vertical));
}
  
  
/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Conversation::emitSendMessageFromConversationWidget() {
  emit sendMessageFromConversationWidget(this);
  
}

} /* end of Widget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */










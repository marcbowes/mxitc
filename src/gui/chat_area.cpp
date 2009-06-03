

#include "chat_area.h"
#include <QWebFrame>


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

ChatArea::ChatArea(QWidget * parent ) : QWidget(parent) {
  setupUi(this);
  
  chatWebView->setFocusProxy(chatInput);
  
  connect(chatInput,   SIGNAL(returnPressed ()), this, SLOT(emitSendMessageFromChatInput()));
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

ChatArea::~ChatArea() {

}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void ChatArea::loadFinished ( bool ok ) {
  
  QWebFrame * frame = chatWebView->page ()->currentFrame ();
  frame->setScrollBarValue(Qt::Vertical, frame->scrollBarMaximum(Qt::Vertical));
}
  
  
/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void ChatArea::emitSendMessageFromChatInput() {
  emit sendMessageFromChatInput(this);
  
}

} /* end of GUI namespace */

} /* end of MXit namespace */










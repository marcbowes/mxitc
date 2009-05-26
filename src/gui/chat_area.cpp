

#include "chat_area.h"


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
  
  connect(chatInput,  SIGNAL(returnPressed ()), this, SLOT(emitSendMessageFromChatInput()));
  connect(chatInput,  SIGNAL(returnPressed ()), chatInput, SLOT(clear()));
  
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

void ChatArea::emitSendMessageFromChatInput() {
  emit sendMessageFromChatInput(this);
  
}

} /* end of GUI namespace */

} /* end of MXit namespace */










/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** 
**
****************************************************************************/

#ifndef __MXIT_GUI_WIDGET_CONVERSATION_H__
#define __MXIT_GUI_WIDGET_CONVERSATION_H__

#include <QWidget>
#include "ui_conversation.h"

namespace MXit
{

namespace GUI
{

namespace Widget
{

class Conversation : public QWidget, public Ui::ChatArea {
  Q_OBJECT
  
  
  public: /* class specific */
  
  Conversation(QWidget * parent = 0);
  //ChatArea(const ChatArea& chatArea);
  ~Conversation();
  //ChatArea& operator=(const ChatArea& chatArea);

  signals:
  
  void sendMessageFromConversationWidget(const Widget::Conversation *);

  private slots:
  void loadFinished ( bool ok );

  void emitSendMessageFromConversationWidget();
  public: /* methods */

  private: /* methods */

  private: /* variables */

};


} /* end of Widget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */

#endif /* __MXIT_GUI_CHAT_AREA_CONTROLLER_H__ */











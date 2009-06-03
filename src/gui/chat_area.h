/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::ChatAreaControl is the class that hold all the information about the different chat area
**
****************************************************************************/

#ifndef __MXIT_GUI_CHAT_AREA_H__
#define __MXIT_GUI_CHAT_AREA_H__

#include <QWidget>
#include "ui_chat_area.h"

namespace MXit
{

namespace GUI
{

class ChatArea : public QWidget, public Ui::ChatArea {
  Q_OBJECT
  
  
  public: /* class specific */
  
  ChatArea(QWidget * parent = 0);
  //ChatArea(const ChatArea& chatArea);
  ~ChatArea();
  //ChatArea& operator=(const ChatArea& chatArea);

  signals:
  
  void sendMessageFromChatInput(const ChatArea *);

  private slots:
  void loadFinished ( bool ok );

  void emitSendMessageFromChatInput();
  public: /* methods */

  private: /* methods */

  private: /* variables */

};


} /* end of GUI namespace */

} /* end of MXit namespace */

#endif /* __MXIT_GUI_CHAT_AREA_CONTROLLER_H__ */











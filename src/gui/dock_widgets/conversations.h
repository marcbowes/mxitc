/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::GUI::Conversation is a QDockWidget for displaying and managing
**  a set of MXit::Conversations.
**
****************************************************************************/

#ifndef __MXIT_GUI_CONVERSATION_H__
#define __MXIT_GUI_CONVERSATION_H__

#include "gui/mxit_dock_widget.h"

#include "ui_chat_sessions.h"

namespace MXit
{

namespace GUI
{

class Conversations : public MXitDockWidget, private Ui::ChatSessionsDockWidget
{
  Q_OBJECT
  
  public:         /* class specific */

  Conversations(QWidget *parent, Theme &theme);
  ~Conversations();
  
  public:         /* methods */
  
  
};

}

}

#endif /* __MXIT_GUI_CONVERSATION_H__ */


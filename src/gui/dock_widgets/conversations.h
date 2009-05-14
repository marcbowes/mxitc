/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::GUI::Conversations is a QDockWidget for displaying
**  MXit::Conversations.
**
****************************************************************************/

#ifndef __MXIT_GUI_CONVERSATIONS_H__
#define __MXIT_GUI_CONVERSATIONS_H__

#include <QHash>

#include "mxit/conversation.h"

namespace MXit
{

namespace GUI
{

class Conversations : public MXitDockWidget, private Ui::ConversationsDockWidget
{
  Q_OBJECT
  
  public:         /* class specific */

  Conversations(QWidget *parent, Theme &theme);
  ~Conversations();
  
  signals:
  
  void conversationSelected(QListWidgetItem*);
  void contextMenuRequest(QListWidgetItem*);
  
  public:         /* methods */
  
  void newConversation(const ContactSet &contacts);
  void newConversation(const ContactSet &contacts, const QString &roomName);
  
  public:        /* variables */
  
  QHash<QString, Conversation*> conversations;

  private slots:
  
  void conversationContextMenuRequest(const QPoint &point);
};

}

}

#endif /* __MXIT_GUI_CONVERSATIONS_H__ */


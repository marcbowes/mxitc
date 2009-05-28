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

#include "mxit/address_book.h"
#include "mxit/conversations.h"
#include "mxit/conversation.h"
#include "mxit/client.h"

#include "gui/mxit_dock_widget.h"

#include "ui_conversations.h"

namespace MXit
{

namespace GUI
{

namespace DockWidget
{

class Conversations : public MXitDockWidget, private Ui::ConversationsDockWidget
{
  Q_OBJECT
  
  public:         /* class specific */

  Conversations(QWidget *parent, Theme &theme, MXit::Client& mxit, MXit::Conversations & conversations);
  ~Conversations();
  
  
  signals:
  
  void outgoingConversationRequest            (const Conversation *conversation);
  void outgoingConversationShowRequest        (const Conversation *conversation);
  void outgoingConversationCloseRequest       (const Conversation *conversation);
  void outgoingConversationReadNotification   (const Conversation *conversation);
  void outgoingMessage                        (QString& message, const Conversation *conversation);

  public slots:

  void incomingConversationRequest            (const Conversation *conversation);
  void incomingConversationCloseRequest       (const Conversation *conversation);
  void incomingConversationShowRequest        (const Conversation *conversation);
  void incomingConversationReadNotification   (const Conversation *conversation);
  void incomingConversationUpdated            (const Conversation *conversation);
  
  
  private slots:
  
  void emitConversationShowRequest(QListWidgetItem *lwi);
  
  void popUpContextMenu(const QPoint & pos);
  
  public slots:
  
  void refreshThemeing();
  
  private:
  
  void refresh(const MXit::OrderedConversationMap& conversations);
  void setConversationCss();
  
  void refreshListWidgetItem(QListWidgetItem* lwi);
  
  
  private:
  
  MXit::Conversations& conversations;
  MXit::Client& mxit;
  
  
  QHash<const MXit::Conversation*, QListWidgetItem*> conversationToLwi;
  QHash<QListWidgetItem*, const MXit::Conversation*> lwiToConversation;
  
};

} /* end of DockWidget namespace */

}

}

#endif /* __MXIT_GUI_CONVERSATIONS_H__ */


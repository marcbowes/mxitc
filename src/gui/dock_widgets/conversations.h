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
  
  //void outgoingItemPressed ( QListWidgetItem *  ); // depricated maybe
  
  
  void chatRequest ( Conversation * conversation );
  
  public:
  
  void refresh(const MXit::OrderedConversationMap& conversations);
  
  private:
  
  void refreshListWidgetItem(QListWidgetItem* lwi);

  public slots:
  
  void refreshThemeing();

  private slots:
  
  void emitChatRequest(QListWidgetItem *lwi);
  
  void conversationsUpdated(const MXit::ConversationList& conversations);
  void popUpContextMenu(const QPoint & pos);
  
  
  private:
  
  MXit::Conversations& conversations;
  MXit::Client& mxit;
  
  
  QHash<MXit::Conversation*, QListWidgetItem*> conversationToLwi;
  QHash<QListWidgetItem*, MXit::Conversation*> lwiToConversation;
  
};

} /* end of DockWidget namespace */

}

}

#endif /* __MXIT_GUI_CONVERSATIONS_H__ */


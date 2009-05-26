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
  void conversationRemovedFromGUI ( const Conversation * conversation  );
  
  void conversationRequest ( const Conversation * conversation );
  
  public:
  
  void conversationRead(const Conversation * conversation);
  void setConversationCss();
  void refresh(const MXit::OrderedConversationMap& conversations);
  
  private:
  
  void refreshListWidgetItem(QListWidgetItem* lwi);

  public slots:
  
  void selectConversation(const Conversation *);
  void refreshThemeing();

  private slots:
  
  void emitConversationRequest(QListWidgetItem *lwi);
  
  void conversationUpdated(const Conversation* conversation);
  void popUpContextMenu(const QPoint & pos);
  
  
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


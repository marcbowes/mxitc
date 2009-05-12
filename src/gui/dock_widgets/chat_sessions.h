/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Richard Baxter, 2009
**
****************************************************************************/

#ifndef __MXIT_GUI_DOCKWIDGET_CHATSESSIONS_H__
#define __MXIT_GUI_DOCKWIDGET_CHATSESSIONS_H__

#include "common/chat_session.h"
#include "gui/mxit_dock_widget.h"

#include "ui_chat_sessions.h"

#include <QDebug>
#include <QSet>

namespace MXit
{

namespace GUI
{

namespace DockWidget
{

class ChatSessions : public MXitDockWidget, private Ui::ChatSessionsDockWidget
{
  Q_OBJECT
  
  public: /*class specific */

  ChatSessions(QWidget* parent, Theme &theme);
  ~ChatSessions();
  
  signals:
  
  void outgoingItemPressed ( QListWidgetItem *  );
  
  void contextMenuRequest(const QPoint &, const QString& chatSession);
  
  
  public:
  
  void refresh(const QList<ChatSession>& chatSessions);
  
  void selectItem(const QString& chatSessionName);

  private slots:
  
  void chatSessionsListContextMenuRequest(const QPoint & pos);
  
  
  private:
  
  //void clearList();
  QListWidgetItem * addChatSession(const ChatSession & c);
  
  QHash<QString, QListWidgetItem*> listItemWidgets; // from chatSessionName to listItemWidget
  

};

} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif


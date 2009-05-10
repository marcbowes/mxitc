/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::OptionsDockWidget provides a login window.
** This is an extension of the generated dialog, made with Designer.
**
****************************************************************************/

#ifndef __MXIT_GUI_DOCKWIDGET_CHATSESSIONS_H__
#define __MXIT_GUI_DOCKWIDGET_CHATSESSIONS_H__

#include "gui/chat_session.h"
#include "gui/mxit_dock_widget.h"

#include "ui_contacts.h"

#include <QDebug>
#include <QSet>

namespace MXit
{

namespace GUI
{

namespace DockWidget
{

class ChatSessions : public MXitDockWidget, private Ui::ContactsDockWidget
{
  Q_OBJECT
  
  public: /*class specific */

  ChatSessions(QWidget* parent, Theme &theme);
  ~ChatSessions();
  
  signals:
  
  void outgoingItemPressed ( QListWidgetItem *  );
  
  
  public:
  
  void refresh(const QList<ChatSession>& chatSessions);

  private:
  
  //void clearList();
  QListWidgetItem * addChatSession(const ChatSession & c);
  
  QHash<QString, QListWidgetItem*> listItemWidgets; // from chatSessionName to listItemWidget

};

} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif



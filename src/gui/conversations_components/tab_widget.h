/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** 
**
****************************************************************************/

#ifndef __MXIT_GUI_CONVERSATIONSCOMPONENTS_TABWIDGET_H__
#define __MXIT_GUI_CONVERSATIONSCOMPONENTS_TABWIDGET_H__


#include "gui/widgets/conversation.h"

#include "mxit/client.h"
#include "mxit/conversations.h"
#include "gui/theme.h"

#include <QTabWidget>

namespace MXit
{

namespace GUI
{

namespace ConversationsComponent
{


class TabWidget : public QTabWidget{
  Q_OBJECT

  public: /* class specific */
  TabWidget(Theme &theme, MXit::Client &mxit, Conversations& conversations, AddressBook& addressBook, QWidget* parent = 0);
  ~TabWidget();
  
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
  void incomingConversationUpdatedFinished    (const Conversation *conversation);
  
  
  private slots:

  void currentChanged ( int index );
  void tabCloseRequested ( int index );
  void sendMessageFromConversationWidget (const Widget::Conversation * conversationWidget);
  
  void removeAndDeleteConversationFromGUI( const MXit::Conversation* conversation );
  
  
  public slots:
  
  void refreshThemeing();
  
  private: /* methods */
  

  void updateTabOf( Widget::Conversation * conversationWidget);

  void switchToConversationTab(const Conversation * Conversation); /* for compatibility with contacts list*/
  
  const Conversation * ensureExistanceOfConversation( const Conversation* conversation );
  
  Widget::Conversation* ensureExistanceOfConversationWidgetAndTab( const Conversation* conversation );
  
  
  
  private: /* variables */
  
  QHash<const Conversation*, Widget::Conversation*> conversationToConversationWidget;
  QHash<const QWidget*, const Conversation*> conversationWidgetToConversation;
  
  MXit::Client &mxit;
  Theme &theme;
  
  Conversations& conversations;
  AddressBook& addressBook;
  
  bool nonUserTabChange;
};


} /* end of ConversationsComponent namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif /* __MXIT_GUI_CONVERSATION_TAB_WIDGE_H__ */


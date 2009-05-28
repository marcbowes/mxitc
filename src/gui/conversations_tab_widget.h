#ifndef __MXIT_GUI_CONVERSATION_TAB_WIDGET_H__
#define __MXIT_GUI_CONVERSATION_TAB_WIDGET_H__


#include "gui/conversations_widget.h"
#include "gui/chat_area.h"

#include "mxit/client.h"
#include "gui/theme.h"

#include <QTabWidget>

namespace MXit
{

namespace GUI
{


class ConversationsTabWidget : public QTabWidget, public ConversationsWidget{
  Q_OBJECT

  public: /* class specific */
  ConversationsTabWidget(Theme &theme, MXit::Client &mxit, Conversations& conversations, AddressBook& addressBook, QWidget* parent = 0);
  ~ConversationsTabWidget();
  
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
  void sendMessageFromChatArea (const ChatArea * chatArea);
  
  void removeAndDeleteConversationFromGUI( const Conversation* conversation );
  
  
  private: /* methods */
  

  void updateTabOf(ChatArea * chatArea);

  void switchToConversationTab(const Conversation * Conversation); /* for compatibility with contacts list*/
  
  const Conversation * ensureExistanceOfConversation( const Conversation* conversation );
  
  ChatArea* ensureExistanceOfChatAreaAndTab( const Conversation* conversation );
  
  
  
  private: /* variables */
  
  QHash<const Conversation*, ChatArea*> conversationToChatArea;
  QHash<const QWidget*, const Conversation*> chatAreaToConversation;
  
  MXit::Client &mxit;
  Theme &theme;
  
  bool nonUserTabChange;
};



} /* end of GUI namespace */

} /* end of MXit namespace */



#endif /* __MXIT_GUI_CONVERSATION_TAB_WIDGE_H__ */


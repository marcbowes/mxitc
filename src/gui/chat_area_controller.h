/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::ChatAreaControl is the class that hold all the information about the different chat area
**
****************************************************************************/

#ifndef __MXIT_GUI_CHAT_AREA_CONTROLLER_H__
#define __MXIT_GUI_CHAT_AREA_CONTROLLER_H__


#include "mxit/client.h"
#include "mxit/contact.h"
#include "mxit/address_book.h"
#include "mxit/conversations.h"

#include "gui/chat_area.h"
#include "gui/theme.h"

#include <QTabWidget>
#include <QUrl>
#include <QWebView>
#include <QWebFrame>


namespace MXit
{

namespace GUI
{

class ChatAreaController : public QObject {
  Q_OBJECT
  
  
  public: /* class specific */
  
  ChatAreaController(Theme &theme, MXit::Client &mxit, Conversations& conversations, AddressBook& addressBook);
  ~ChatAreaController();

  signals:
  void conversationRequested(const Conversation *);

  void outgoingConversationRequest(const Conversation *);
  public slots:
  
  
  /*DOES NOTHING! FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME*/
  void incomingConversationUpdate(const Conversation *);
  void incomingConversationRequest(const Conversation *);
  
  
  
  
  private slots:
  void sendMessageFromChatArea (const ChatArea * chatArea);

  void emitConversationRequested ( int index );
  
  void removeAndDeleteConversationFromGUI( int index );

  public: /* methods */
  void updateTabOf(const  Conversation *);

  void switchToConversationTab(const Contact * contact);
  void switchToConversationTab(const QString & uniqueId);
  void switchToConversationTab(const Conversation * Conversation); /* for compatibility with contacts list*/
  
  void removeAndDeleteConversationFromGUI( const Conversation* conversation );
  QWidget * getCentralChatArea();
  
  /*FIXME these methods should be in Conversations*/
  //const Conversation * ensureExistanceOfConversation(const Conversation * conversation);
  //const Conversation * ensureExistanceOfConversation(const Contact * contact);
  const Conversation * ensureExistanceOfConversation(const QString & uniqueId);
  
  private: /* methods */
  
  
  ChatArea* ensureExistanceOfChatAreaAndTab(const QString & uniqueId);
  
  
  void outgoingMessage(const QString & message, const Conversation * conversation);

  

  private: /* variables */
  MXit::Client &mxit;
  Theme &theme;
  Conversations &conversations;
  AddressBook &addressBook;
  
  QTabWidget* chatTabWidget;
  QHash<const Conversation*, ChatArea*> conversationToChatArea;
  QHash<const QWidget*, const Conversation*> chatAreaToConversation;

};


} /* end of GUI namespace */

} /* end of MXit namespace */

#endif /* __MXIT_GUI_CHAT_AREA_CONTROLLER_H__ */











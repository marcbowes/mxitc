/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::ChatAreaControl is the class that hold all the information about the different chat area
**
****************************************************************************/

#ifndef __MXIT_GUI_CONVERSATIONS_WIDGETS_CONTROLLER_H__
#define __MXIT_GUI_CONVERSATIONS_WIDGETS_CONTROLLER_H__


#include "mxit/client.h"
#include "mxit/contact.h"
#include "mxit/address_book.h"
#include "mxit/conversations.h"

#include "gui/chat_area.h"
#include "gui/theme.h"

#include <QUrl>
#include <QWebView>
#include <QWebFrame>


namespace MXit
{

namespace GUI
{

class ConversationsWidgetsController : public QObject {
  Q_OBJECT
  
  
  public: /* class specific */
  
  ConversationsWidgetsController(Theme &theme, MXit::Client &mxit, Conversations& conversations, AddressBook& addressBook);
  ~ConversationsWidgetsController();


  signals:
  
  //void incomingConversationUpdatesToWidgets(const Conversation *);
  void incomingConversationShowRequestsToWidgets(const Conversation *);
  
  public slots:
  
  //void incomingConversationUpdate(const Conversation *);
  
  void incomingConversationShowRequest (const Contact *contact);
  
  const Conversation * ensureExistanceAndActivationOfConversation(const QString& uniqueId);

  private slots:
  
  void incomingMessageFromWidget(QString& message, const Conversation * conversation);
  
  public: /* methods */
  
  void addConversationsWidget(QWidget * conversationsWidget);
  

  private: /* variables */
  
  QVector<QWidget*> conversationsWidgets;
  
  MXit::Client &mxit;
  Theme &theme;
  Conversations &conversations;
  AddressBook &addressBook;

};


} /* end of GUI namespace */

} /* end of MXit namespace */

#endif /* __MXIT_GUI_CHAT_AREA_CONTROLLER_H__ */











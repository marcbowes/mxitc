/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** ConvesationsComponent Controller, controls the various conversation views
**
****************************************************************************/

#ifndef __MXIT_GUI_CONVERSATIONSCOMPONENTS_CONTROLLER_H__
#define __MXIT_GUI_CONVERSATIONSCOMPONENTS_CONTROLLER_H__


#include "mxit/client.h"
#include "mxit/contact.h"
#include "mxit/address_book.h"
#include "mxit/conversations.h"

#include "gui/widgets/conversation.h"
#include "gui/theme.h"


namespace MXit
{

namespace GUI
{

namespace ConversationsComponent
{

class Controller : public QObject {
  Q_OBJECT
  
  
  public: /* class specific */
  
  Controller(Theme &theme, MXit::Client &mxit, Conversations& conversations, AddressBook& addressBook);
  ~Controller();


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


} /* end of ConversationsComponent namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */

#endif /* __MXIT_GUI_CHAT_AREA_CONTROLLER_H__ */











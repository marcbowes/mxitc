/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
** Author: Richard Baxter, 2009
**
** GUI flavour of a contact (extends common/contact)
** provides functionality to display & chat with a contact
**
****************************************************************************/

#ifndef __MXIT_CHATSESSION_H__
#define __MXIT_CHATSESSION_H__

#include "types.h"

#include "mxit/contact.h"

#include "gui/message.h"

#include <QTableWidget>

namespace MXit
{

using GUI::Message; /*FIXME remove when Message class is moved to common*/
using GUI::MessageVec; /*FIXME remove when Message class is moved to common*/

class ChatSession
{
  public:         /* class specific */
  
  ChatSession(MXit::Contact * mainContact = 0);
  //ChatSession(const ChatSession &other);
  ~ChatSession();
  
  public:         /* methods */
  
  //ChatSession& operator=(const ChatSession &other);
  
  void incomingMessage(Message message);
  
  public:         /* variables */
  
  QString chatSessionName;
  
  MXit::Contact * mainContact; /*slight hack for now since we will always have one contact, will have to be changed later*/
  //QSet<MXit::Contact *> otherContacts; /*TODO implement*/
  
  //QTableWidget tableWidget;
  MessageVec  chatHistory; /*replace with QTableWidget*/
  QString     chatInputText;
  bool        unreadMessage;
  
  
};

}

#endif /* __MXIT_GUI_CONTACT_H__ */


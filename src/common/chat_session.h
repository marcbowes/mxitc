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
#include "mxit/message.h"

#include <QTableWidget>

namespace MXit
{

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
  MessageList chatHistory; /*replace with QTableWidget*/
  QString     chatInputText;
  bool        unreadMessage;
  
  
};

}

#endif /* __MXIT_GUI_CONTACT_H__ */


/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "chat_session.h"

namespace MXit
{

/****************************************************************************
**
** Author: Marc Bowes
**
** Contact constructor
**
****************************************************************************/
ChatSession::ChatSession(MXit::Contact * mainContact)
{
  this->mainContact = mainContact;
  
  if (this->mainContact) {
    chatSessionName = this->mainContact->nickname;
  }
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Contact copy constructor
**
****************************************************************************/
//ChatSession::ChatSession(const ChatSession &other)
//{
  /**/
//}


/****************************************************************************
**
** Author: Marc Bowes
**
** Contact deconstructor
**
****************************************************************************/
ChatSession::~ChatSession()
{
  Q_FOREACH(Message *message, chatHistory)
    delete message;
}


/*Contact& ChatSession::operator=(const Contact &other)
{

  return *this;
}*/


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void ChatSession::incomingMessage(Message message)
{
  chatHistory.append(new Message(message));
  
  unreadMessage = true;
}


}












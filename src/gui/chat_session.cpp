/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "chat_session.h"

namespace MXit
{

namespace GUI
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
  /* nothing */
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
  chatHistory.append(message);
  
  unreadMessage = true;
}

}

}












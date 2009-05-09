/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QDebug>

#include "message.h"
#include "contact.h"

namespace MXit
{


namespace GUI
{

/****************************************************************************
**
** Author: Richard Baxter
**
** Contact default constructor
**
****************************************************************************/

Message::Message(const Contact * sender, const QString & message) : 
      sender(sender), message(message)
{
  /* nothing */
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Contact copy constructor
**
****************************************************************************/

Message::Message(const Message & m) :  
      sender(m.sender), message(m.message)
{
  /* nothing */
}

/****************************************************************************
**
** Author: Richard Baxter
**
** = operator
**
****************************************************************************/

Message& Message::operator=(const Message & m)
{
  sender = m.sender;
  message = m.message;
  
  return *this;
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Contact deconstructor
**
****************************************************************************/
Message::~Message()
{
  /* nothing */
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

const Contact * Message::getSender() const
{
  return sender;
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

QString Message::getMessage() const
{
  return message;
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

QString Message::getFormattedMsg() const
{
  return (sender?sender->nickname:QString("You")) + ": " + message;
}

} /* end namespace GUI */


} /* end namespace MXit */



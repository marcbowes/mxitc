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
      senderVar(sender), messageVar(message)
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
      senderVar(m.senderVar), messageVar(m.messageVar)
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
  senderVar = m.senderVar;
  messageVar = m.messageVar;
  
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

const Contact * Message::sender() const
{
  return senderVar;
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

QString Message::message() const
{
  return messageVar;
}


} /* end namespace GUI */


} /* end namespace MXit */



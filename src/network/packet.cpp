/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "packet.h"

namespace MXit
{

namespace Network
{

/****************************************************************************
**
** Author: Richard Baxter
**
** Packet constructor
**
****************************************************************************/
Packet::Packet(const QString &cellphone , const QString &commandNumber)
  : cellphone (cellphone), command (commandNumber)
{
  // nothing else
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Packet deconstructor -- required for inheritance to work properly
**
****************************************************************************/
Packet::~Packet()
{
  // nothing
}

/****************************************************************************
**
** Author: Marc Bowes
**
** turns the data vector into a single QString seperated by \1
**
****************************************************************************/
QString Packet::getData() const
{
  QString message;
  StringVecItr itr(data);
  
  while (itr.hasNext()) {
    const QString &d = itr.next();
    message.append(d);
    if (itr.hasNext()) /* append \1 if there is another message */
      message.append("\1");
  }
  
  return message;
}


/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/
void Packet::setCellphone(const QString &cellphone)
{
  this->cellphone = cellphone;
}


/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/
void Packet::setCommand(const QString &command)
{
  this->command = command;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** appends the message to this packet's data
**
****************************************************************************/
Packet& Packet::operator<<(const QString &message)
{
  data.append(message);
  return *this;
}

}

}


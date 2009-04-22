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
  return data.join("\1");
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
****************************************************************************/
void Packet::setMsTerminator(char terminator)
{
  msTerminator = terminator;
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
  data << message;
  return *this;
}

}

}


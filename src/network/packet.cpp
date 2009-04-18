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
** Author: Marc Bowes
**
** appends the message to this packet's data
**
****************************************************************************/
void Packet::operator<<(const QString &message)
{
  data.append(message);
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

}

}

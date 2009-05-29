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
QByteArray Packet::getData() const
{
  QByteArray _data;
  QListIterator<QByteArray> itr (data);
  while (itr.hasNext()) {
    const QByteArray &i = itr.next();
    _data.append(i);
    if (itr.hasNext())
      _data.append('\1');
  }
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
void Packet::setPostMs(const QByteArray &extra)
{
  this->extra = extra;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** appends the message to this packet's data
**
****************************************************************************/
Packet& Packet::operator<<(const QByteArray &message)
{
  data << message;
  return *this;
}

}

}


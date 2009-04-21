/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "tcp.h"

namespace MXit
{

namespace Network
{

namespace Packets
{

/****************************************************************************
**
** Author: Richard Baxter
**
** TCPPacket constructor
**
****************************************************************************/
TCP::TCP(const QString &cellphone, const QString &commandNumber)
 : Packet(cellphone, commandNumber)
{
  // nothing here
}


/****************************************************************************
**
** Author: Marc Bowes
**
** TCPPacket constructor
**
****************************************************************************/
TCP::~TCP()
{
  // nothing here
}


/****************************************************************************
**
** Author: Marc Bowes
**
** turns this TCPPacket into a QByteArray:
**
**  "ln"=nnn\0
**  "id"=loginname\0
**  "cm"=cmdNr\0
**  "ms"=msg0\1msg\1...\1msgN
**
** where
**  nnn         is the length (in bytes) of the request as measured from after
**              the record terminator of the "ln" record.
**  loginname   is the unique username that identifies the user. Currently the
**              user's cell number.
**  cmdNr       is the number that identifies the command uniquely.
**  msgN        is the command specific message data that is interpreted by
**              MXit according to the command number.
**
****************************************************************************/
TCP::operator QByteArray() const
{
  QByteArray self;
  self.append   (     QString("\"id\"=%1\0")  .arg(cellphone)             );
  self.append   (     QString("\"cm\"=%1\0")  .arg(command)               );
  self.append   (     QString("\"ms\"=%1")    .arg(getData())             );
  self.prepend  (     QString("\"ln\"=%1\0")  .arg(self.size()).toLatin1());
  /* dont worry about charset as an int is an int no matter what :) */
  
  return self;
}

}

}

}


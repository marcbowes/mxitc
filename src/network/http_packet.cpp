/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "http_packet.h"

namespace MXit
{

namespace Network
{


/****************************************************************************
**
** Author: Richard Baxter
**
** Consructor to set all variables
**
****************************************************************************/

  HTTPPacket::HTTPPacket(unsigned int seqNo, int sessID, QString cellph, QString cmd) : 
    Packet(cellph, cmd), sequenceNo(seqNo), sessionID(sessID) {}
    
    
/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
int HTTPPacket::getSessionID() {
  return sessionID;
}
  
  
/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

int HTTPPacket::getSequenceNo(){
  return sequenceNo;
}

/****************************************************************************
**
** Author: Richard Baxter
** Author/stolen from: Marc Bowes
**
** turns this HTTPPacket into a QByteArray:
**
**  "id"=loginname &
**  "s"=[ sesid \1 ] seqno &
**  "cm"=cmdNr &
**  "ms"=msg0 \1 msg1 \1 ... \1 msgN
**
** where
**  loginname   is the unique username that identifies the user. Currently the 
**              user’s cell number.
**  s           is the session information.
**  sesid       is a session ID that is returned upon completion of a 
**              login/register call
**  seqno       is a session specific number that is incremented each time a 
**              command is executed.
**  cmdNr       is the number that identifies the command uniquely.
**  msgN        is the command specific message data that is interpreted by
**              MXit according to the command number.
**
****************************************************************************/
HTTPPacket::operator QByteArray() const
{
  QByteArray self;
  self.append   (     QString("\"id\"=%1&")   .arg(cellphone)             );
  
  /* next three lines of code creates the {"s"=[ sesid \1 ] seqno &} part */
  self.append   (     QString("\"s\"=")                                   );
  if (sessionID != -1)
    self.append   (     QString("%1\1")         .arg(sessionID)             );
  self.append   (     QString("%1&")          .arg(sequenceNo)            );
    
  self.append   (     QString("\"cm\"=%1&")   .arg(command)               );
  self.append   (     QString("\"ms\"=%1")    .arg(getData())             );
  
  return self;
}

}

}

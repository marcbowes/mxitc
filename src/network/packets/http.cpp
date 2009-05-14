/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QUrl>

#include "http.h"

namespace MXit
{

namespace Network
{

namespace Packets
{

static unsigned int sequenceCounter = 0;

/****************************************************************************
**
** Author: Richard Baxter
**
** Consructor to set all variables
**
****************************************************************************/
HTTP::HTTP(int sessionID, const QString &cellphone, const QString &commandNumber)
  : Packet(cellphone, commandNumber)
{
  sequenceNumber = sequenceCounter++;
  sessionID = sessionID;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** HTTPPacket constructor
**
****************************************************************************/
HTTP::~HTTP()
{
  // nothing here
}

    
/****************************************************************************
**
** Author: Richard Baxter
**
** turns this HTTPPacket into a QByteArray:
**
**  id=loginname &
**  s=[ sesid \1 ] seqno &
**  cm=cmdNr &
**  ms=msg0 \1 msg1 \1 ... \1 msgN
**
** where
**  loginname   is the unique username that identifies the user. Currently the 
**              user's cell number.
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
HTTP::operator QByteArray() const
{
  QByteArray self;
  self.append   ( QString("id=%1&")   .arg(cellphone)      );
  
  /* next three lines of code creates the {"s"=[ sesid \1 ] seqno &} part */
  self.append   ( QString("s=")                            );
  if ((sessionID > 0)&&(command != "1")) {
    self.append ( QString("%1")    .arg(sessionID)      );
    self.append ( "%01" );
  }
  self.append   ( QString("%1&")      .arg(sequenceNumber) );
  self.append   ( QString("cm=%1")    .arg(command)        );
  
  QString ms = QUrl::toPercentEncoding(getData()).replace('\1', "%01");
  if (!ms.isEmpty())
    self.append ( QString("&ms=%1")    .arg(ms)      );
  
  self.append   ( extra );
  self.append   ("&"); /* HTTP record terminator */
  
  return self;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** sets the HTTP session id
**
****************************************************************************/
void HTTP::setSessionID(int sessionID)
{
  this->sessionID = sessionID;
}

}

}

}


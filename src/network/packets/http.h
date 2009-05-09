/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
** Author: Richard Baxter, 2009
**
** MXit::Network::Packets::HTTP implements a MXit::Network::Packet for HTTP
**
****************************************************************************/

#ifndef __MXIT_NETWORK_PACKETS_HTTP_H__
#define __MXIT_NETWORK_PACKETS_HTTP_H__

#include "network/packet.h"

namespace MXit
{

namespace Network
{

namespace Packets
{

class HTTP : public Packet
{
  public:           /* class specific */
  
  HTTP(int sessionID = -1, const QString &cellphone = "", const QString &commandNumber = "");
  virtual ~HTTP();
  
  public:           /* methods */
  
  virtual operator QByteArray() const;
  void setSessionID(int sessionID);
  
  protected:
  
  int sessionID; /* if no session number is to be sent, this should be set to -1 */
  unsigned int sequenceNumber;
};

}

}

}

#endif


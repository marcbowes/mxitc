/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Network::TCPPacket implements a MXit::Network::Packet for TCP use
**
****************************************************************************/

#ifndef __MXIT_NETWORK_HTTP_PACKET_H__
#define __MXIT_NETWORK_HTTP_PACKET_H__

#include "packet.h"

namespace MXit
{

namespace Network
{

class HTTPPacket : public Packet
{
  public:           /* class specific */
  
  HTTPPacket(unsigned int seqNo, int sessID = -1, QString cellph = "", QString cmd = "");
  ~HTTPPacket();
  
  public:           /* methods */
  
  int getSessionID();
  int getSequenceNo();
  virtual operator QByteArray() const;
  
  protected:
  
  int sessionID; /* if no session number is to be sent, this should be set to -1 */
  unsigned int sequenceNo;
};

}

}

#endif


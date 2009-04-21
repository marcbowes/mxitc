/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
** Author: Richard Baxter, 2009
**
** MXit::Network::TCPPacket implements a MXit::Network::Packet for TCP use
**
****************************************************************************/

#ifndef __MXIT_NETWORK_TCP_PACKET_H__
#define __MXIT_NETWORK_TCP_PACKET_H__

#include "network/packet.h"

namespace MXit
{

namespace Network
{

class TCPPacket : public Packet
{
  public:           /* class specific */
  
  TCPPacket(const QString &cellphone = "", const QString &commandNumber = "");
  virtual ~TCPPacket();
  
  public:           /* methods */
  
  virtual operator QByteArray() const;
};

}

}

#endif


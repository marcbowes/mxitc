/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
** Author: Richard Baxter, 2009
**
** MXit::Network::Packets::TCP implements a MXit::Network::Packet for TCP
**
****************************************************************************/

#ifndef __MXIT_NETWORK_PACKETS_TCP_H__
#define __MXIT_NETWORK_PACKETS_TCP_H__

#include <QDebug>

#include "network/packet.h"

namespace MXit
{

namespace Network
{

namespace Packets
{

class TCP : public Packet
{
  public:           /* class specific */
  
  TCP(const QString &cellphone = "", const QString &commandNumber = "");
  virtual ~TCP();
  
  public:           /* methods */
  
  virtual operator QByteArray() const;
};

}

}

}

#endif


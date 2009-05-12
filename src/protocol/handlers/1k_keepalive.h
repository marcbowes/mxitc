/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Network::Handlers::KeepAlive is a packet to prevent connection
** drops (see #43)
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_KEEPALIVE_H__
#define __MXIT_PROTOCOL_HANDLERS_KEEPALIVE_H__

#include "protocol/handler.h"

#define KEEPALIVE 1000

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class KeepAlive : public Handler
{
  public:
  
  KeepAlive() { command = KEEPALIVE; }
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif


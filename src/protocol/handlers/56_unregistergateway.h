/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::UnregisterGateway undoes 54
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_UNREGISTERGATEWAY_H__
#define __MXIT_PROTOCOL_HANDLERS_UNREGISTERGATEWAY_H__

#include "protocol/handler.h"

#define UNREGISTERGATEWAY 56

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class UnregisterGateway : public Handler
{
  public:
  
  UnregisterGateway() { command = UNREGISTERGATEWAY; }
  
  virtual void buildPacket(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

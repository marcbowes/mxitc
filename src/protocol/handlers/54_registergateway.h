/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::RegisterGateway handles including other IM contacts
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_REGISTERGATEWAY_H__
#define __MXIT_PROTOCOL_HANDLERS_REGISTERGATEWAY_H__

#include "protocol/handler.h"

#define REGISTERGATEWAY 54

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class RegisterGateway : public Handler
{
  public:
  
  RegisterGateway() { command = REGISTERGATEWAY; }
  
  virtual void buildPacket(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::Register deals with registering a new account
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_REGISTER_H__
#define __MXIT_PROTOCOL_HANDLERS_REGISTER_H__

#include "protocol/handler.h"

#define REGISTER 11

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class Register : public Handler
{
  public:
  
  Register() { command = REGISTER; }
  
  virtual void buildPacket(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

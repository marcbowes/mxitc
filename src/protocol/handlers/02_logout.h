/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::Logout deals with logging out of MXit
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_LOGOUT_H__
#define __MXIT_PROTOCOL_HANDLERS_LOGOUT_H__

#include "protocol/handler.h"

#define LOGOUT 2

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class Logout : public Handler
{
  public:
  
  Logout() { command = LOGOUT; }
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
** Author: Richard Baxter, 2009
**
** MXit::Network::Handlers::Login deals with logging into MXit
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_LOGIN_H__
#define __MXIT_PROTOCOL_HANDLERS_LOGIN_H__

#include "protocol/handler.h"

#define LOGIN 1

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class Login : public Handler
{
  public:
  
  Login() { command = LOGIN; }
  
  virtual void buildPacket(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif


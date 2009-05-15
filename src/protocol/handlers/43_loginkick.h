/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Network::Handlers::LoginKick is a handle-only packet and indicates
** the client needs to re-login
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_LOGINKICK_H__
#define __MXIT_PROTOCOL_HANDLERS_LOGINKICK_H__

#include "protocol/handler.h"

#define LOGINKICK 43

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class LoginKick : public Handler
{
  public:
  
  LoginKick() { command = LOGINKICK; }
  
  virtual void buildPacket(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif


/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Network::Handlers::GetNewSubscription is a handle-only packet for
** interpreting friendship requests
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_GETNEWSUBSCRIPTION_H__
#define __MXIT_PROTOCOL_HANDLERS_GETNEWSUBSCRIPTION_H__

#include "protocol/handler.h"

#define GETNEWSUBSCRIPTION 51

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class GetNewSubscription : public Handler
{
  public:
  
  GetNewSubscription() { command = GETNEWSUBSCRIPTION; }
  
  virtual void buildPacket(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif


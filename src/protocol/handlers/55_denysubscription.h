/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Network::Handlers::DenySubscription is a build-only packet for
** authorizing (denying) contacts
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_DENYSUBSCRIPTION_H__
#define __MXIT_PROTOCOL_HANDLERS_DENYSUBSCRIPTION_H__

#include "protocol/handler.h"

#define DENYSUBSCRIPTION 55

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class DenySubscription : public Handler
{
  public:
  
  DenySubscription() { command = DENYSUBSCRIPTION; }
  
  virtual void buildPacket(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif


/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Network::Handlers::BlockSubscription is a build-only packet for
** authorizing (denying) contacts (permanently)
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_BLOCKSUBSCRIPTION_H__
#define __MXIT_PROTOCOL_HANDLERS_BLOCKSUBSCRIPTION_H__

#include "protocol/handler.h"

#define BLOCKSUBSCRIPTION 55

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class BlockSubscription : public Handler
{
  public:
  
  BlockSubscription() { command = BLOCKSUBSCRIPTION; }
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif


/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::RemoveGroup handles exactly what it sounds like
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_REMOVEGROUP_H__
#define __MXIT_PROTOCOL_HANDLERS_REMOVEGROUP_H__

#include "protocol/handler.h"

#define REMOVEGROUP 30

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class RemoveGroup : public Handler
{
  public:
  
  RemoveGroup() { command = REMOVEGROUP; }
  
  virtual void buildPacket(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

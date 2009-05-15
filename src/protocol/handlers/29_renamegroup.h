/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::RenameGroup handles exactly what it sounds like
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_RENAMEGROUP_H__
#define __MXIT_PROTOCOL_HANDLERS_RENAMEGROUP_H__

#include "protocol/handler.h"

#define RENAMEGROUP 29

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class RenameGroup : public Handler
{
  public:
  
  RenameGroup() { command = RENAMEGROUP; }
  
  virtual void buildPacket(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

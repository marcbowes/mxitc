/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::SendMessageToGroup handles sending a message to
** many contacts
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_SENDMESSAGETOGROUP_H__
#define __MXIT_PROTOCOL_HANDLERS_SENDMESSAGETOGROUP_H__

#include "protocol/handler.h"

#define SENDMESSAGETOGROUP 34

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class SendMessageToGroup : public Handler
{
  public:
  
  SendMessageToGroup() { command = SENDMESSAGETOGROUP; }
  
  virtual void buildPacket(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

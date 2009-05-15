/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::CreateNewGroupChat makes a new chatroom
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_CREATENEWGROUPCHAT_H__
#define __MXIT_PROTOCOL_HANDLERS_CREATENEWGROUPCHAT_H__

#include "protocol/handler.h"

#define CREATENEWGROUPCHAT 44

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class CreateNewGroupChat : public Handler
{
  public:
  
  CreateNewGroupChat() { command = CREATENEWGROUPCHAT; }
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::AddNewGroupchatMember deals with logging out of MXit
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_ADDNEWGROUPCHATMEMBER_H__
#define __MXIT_PROTOCOL_HANDLERS_ADDNEWGROUPCHATMEMBER_H__

#include "protocol/handler.h"

#define ADDNEWGROUPCHATMEMBER 45

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class AddNewGroupchatMember : public Handler
{
  public:
  
  AddNewGroupchatMember() { command = ADDNEWGROUPCHATMEMBER; }
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

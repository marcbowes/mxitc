/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::AddContact deals with requesting the addition of
** a new contact
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_ADDCONTACT_H__
#define __MXIT_PROTOCOL_HANDLERS_ADDCONTACT_H__

#include "protocol/handler.h"

#define ADDCONTACT 6

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class AddContact : public Handler
{
  public:
  
  AddContact() { command = ADDCONTACT; }
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

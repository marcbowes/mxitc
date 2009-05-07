/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::UpdateContact deals with updating a specific contacts details
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_UPDATECONTACTINFO_H__
#define __MXIT_PROTOCOL_HANDLERS_UPDATECONTACTINFO_H__

#include "protocol/handler.h"

#define UPDATECONTACTINFO 5

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class UpdateContactInfo : public Handler
{
  public:
  
  UpdateContactInfo() { command = UPDATECONTACTINFO; }
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

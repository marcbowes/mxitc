/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::GetContacts deals with logging out of MXit
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_GETCONTACTS_H__
#define __MXIT_PROTOCOL_HANDLERS_GETCONTACTS_H__

#include <QObject>

#include "protocol/handler.h"

#define GETCONTACTS 3

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class GetContacts : public Handler
{
  public:
  
  GetContacts() { command = GETCONTACTS; }
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::RemoveContact deals with removing a contact
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_REMOVECONTACT_H__
#define __MXIT_PROTOCOL_HANDLERS_REMOVECONTACT_H__

#include <QObject>

#include "protocol/handler.h"

#define REMOVECONTACT 8

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class RemoveContact : public Handler
{
  public:
  
  RemoveContact() { command = REMOVECONTACT; }
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

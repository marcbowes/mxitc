/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::PollDifference deals with retrieving new data on 
** an http connection
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_POLLDIFFERENCE_H__
#define __MXIT_PROTOCOL_HANDLERS_POLLDIFFERENCE_H__

#include "protocol/handler.h"

#define POLLDIFFERENCE 17

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class PollDifference : public Handler
{
  public:
  
  PollDifference() { command = POLLDIFFERENCE; }
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

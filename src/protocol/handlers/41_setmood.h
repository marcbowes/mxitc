/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::SetMood sets the users mood on MXit
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_SETMOOD_H__
#define __MXIT_PROTOCOL_HANDLERS_SETMOOD_H__

#include "protocol/handler.h"

#define SETMOOD 41

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class SetMood : public Handler
{
  public:
  
  SetMood() { command = SETMOOD; }
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

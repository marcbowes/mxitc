/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Network::Handlers::UpdateProfile is a build-only packet
** "updates the user's password and/or nickname and other details"
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_UPDATEPROFILE_H__
#define __MXIT_PROTOCOL_HANDLERS_UPDATEPROFILE_H__

#include "protocol/handler.h"

#define UPDATEPROFILE 12

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class UpdateProfile : public Handler
{
  public:
  
  UpdateProfile() { command = UPDATEPROFILE; }
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif


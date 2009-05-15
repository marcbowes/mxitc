/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::GetProfile deals with retrieving the currect 
** users profile from MXit
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_GETPROFILE_H__
#define __MXIT_PROTOCOL_HANDLERS_GETPROFILE_H__

#include "protocol/handler.h"

#define GETPROFILE 26

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class GetProfile : public Handler
{
  public:
  
  GetProfile() { command = GETPROFILE; }
  
  virtual void buildPacket(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

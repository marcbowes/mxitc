/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::GetMessages deals with retrieving new messages
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_GETMESSAGES_H__
#define __MXIT_PROTOCOL_HANDLERS_GETMESSAGES_H__

#include <QObject>

#include "protocol/handler.h"

#define GETMESSAGES 9

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class GetMessages : public Handler
{
  public:
  
  GetMessages() { command = GETMESSAGES; }
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif

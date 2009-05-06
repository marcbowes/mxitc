/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Network::Handlers::Login deals with logging into MXit
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_SENDNEWMESSAGE_H__
#define __MXIT_PROTOCOL_HANDLERS_SENDNEWMESSAGE_H__

#include <QObject>

#include "protocol/handler.h"

#define SENDNEWMESSAGE 10

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class SendNewMessage : public Handler
{
  public:
  
  SendNewMessage() { command = SENDNEWMESSAGE; }
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif


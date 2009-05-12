/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Network::Handlers::AllowSubscription is a build-only packet for
** authorizing contacts
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_ALLOWSUBSCRIPTION_H__
#define __MXIT_PROTOCOL_HANDLERS_ALLOWSUBSCRIPTION_H__

#include "protocol/handler.h"

#define ALLOWSUBSCRIPTION 52

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class AllowSubscription : public Handler
{
  public:
  
  AllowSubscription() { command = ALLOWSUBSCRIPTION; }
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif


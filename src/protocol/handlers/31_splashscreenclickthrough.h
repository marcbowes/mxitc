/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Network::Handlers::SplashScreenClickthrough is a build-only packet
** for letting MXit know that the splash screen was clicked on
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_SPLASHSCREENCLICKTHROUGH_H__
#define __MXIT_PROTOCOL_HANDLERS_SPLASHSCREENCLICKTHROUGH_H__

#include "protocol/handler.h"

#define SPLASHSCREENCLICKTHROUGH 31

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class SplashScreenClickthrough : public Handler
{
  public:
  
  SplashScreenClickthrough() { command = SPLASHSCREENCLICKTHROUGH; }
  
  virtual void buildPacket(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
};

}

}

}

#endif


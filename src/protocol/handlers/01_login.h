/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
** Author: Richard Baxter, 2009
**
** MXit::Network::Handlers::Login deals with logging into MXit
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_LOGIN_H__
#define __MXIT_PROTOCOL_HANDLERS_LOGIN_H__

#include <QObject>

#include "protocol/handler.h"

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class Login : public Handler
{
  Q_OBJECT
  
  public:         /* methods */
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual void handle(const QByteArray &packet);
  
  public:         /* variables */
  
  static const unsigned int command = 1;
};

}

}

}

#endif


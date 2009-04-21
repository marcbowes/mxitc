/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Network::Handler is a blueprint for building command handlers
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_LOGIN_H__
#define __MXIT_PROTOCOL_HANDLERS_LOGIN_H__

#include <QObject>

#include "protocol/handlers.h"

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class Login : public Handler
{
  Q_OBJECT
  
  signals:
  
  void handled(); /* used to deleteLater() */
  
  public:         /* methods */
  
  virtual void build(MXit::Network::Packet *packet, const VariableHash &variables) = 0;
  virtual void handle(const QByteArray &packet) = 0;
};

}

}

}

#endif


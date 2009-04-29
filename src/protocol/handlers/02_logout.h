/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::Logout deals with logging out of MXit
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_LOGOUT_H__
#define __MXIT_PROTOCOL_HANDLERS_LOGOUT_H__

#include <QObject>

#include "protocol/handler.h"

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class Logout : public Handler
{
  Q_OBJECT
  
  public:         /* methods */
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
  
  public:         /* variables */
  
  static const unsigned int command = 2;
};

}

}

}

#endif

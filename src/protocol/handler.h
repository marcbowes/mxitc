/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Protocol::Handler is a blueprint for building command handlers
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLER_H__
#define __MXIT_PROTOCOL_HANDLER_H__

#include <QObject>
#include <QDebug>

#include "common/hash_variables.h"

#include "network/packet.h"

namespace MXit
{

namespace Protocol
{

class Handler
{
  public:
  
  Handler() : command(0) {}
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables) = 0;
  virtual VariableHash handle(const QByteArray &packet) = 0;
  
  unsigned int command;
};

typedef QHash         <QString, Handler*> HandlerHash;
typedef QHashIterator <QString, Handler*> HandlerHashItr;

}

}

#endif


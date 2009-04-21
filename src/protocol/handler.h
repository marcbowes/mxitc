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
#include <QRegExp>

#include "common/types.h"

#include "network/packet.h"

namespace MXit
{

namespace Protocol
{

class Handler : public QObject
{
  Q_OBJECT
  
  signals:
  
  void outgoingVariables(const VariableHash &variables);
  
  public:         /* methods */
  
  virtual void build(MXit::Network::Packet *packet, const VariableHash &variables) = 0;
  virtual void handle(const QByteArray &packet) = 0;
};

}

}

#endif


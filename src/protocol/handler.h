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

/****************************************************************************
**
** Author: Marc Bowes
**
** extracts the error code from a response
**
****************************************************************************/
static VariableHash packetError(const QByteArray &packet)
{
  /* == Example
   * ln=x\0                           TCP only
   * 1\0
   * errorCode[\1errorMessage]\0
   * ... etc
   */
  
  /* skip ln=x\0 and command */
  int idx0, idx1, idx2;
  idx0 = packet.indexOf('\0');              /* skip either ln or command */
  if (packet.startsWith("ln="))             /* skip was on ln */
    idx0 = packet.indexOf('\0', idx0 + 1);  /* skip command as well */
  idx1 = packet.indexOf('\0', idx0 + 1);    /* \0 after errorCode */
  
  /* errorCode[\1errorMessage] */         /* this is 'len' */
  QByteArray error = packet.mid(idx0 + 1, idx1 - idx0 - 1);
  
  /* now need to check for \1 */
  idx2 = error.indexOf("\1");               /* a \1 indicates an errorMessage */
  
  VariableHash ret;
  if (idx2 == -1) { /* no errorMessage */
    ret["code"] = error;
    ret["message"] = "";
  } else {
    ret["code"] = error.left(idx2);
    ret["message"] = error.mid(idx2 + 1);
  }
  
  return ret;
}

class Handler : public QObject
{
  Q_OBJECT
  
  signals:
  
  void outgoingVariables(const VariableHash &variables);
  
  public:         /* methods */
  
  virtual void build(MXit::Network::Packet *packet, VariableHash &variables) = 0;
  virtual VariableHash handle(const QByteArray &packet) = 0;
  
  static const unsigned int command;
};

typedef QHash         <QString, Handler*> HandlerHash;
typedef QHashIterator <QString, Handler*> HandlerHashItr;

}

}

#endif


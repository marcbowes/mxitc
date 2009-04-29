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
static VariableHash packetHeader(const QByteArray &packet)
{
  /* == Example
   * ln=x\0                           TCP only
   * 1\0
   * errorCode[\1errorMessage]\0
   * ... etc
   */
  VariableHash ret;
  
  /* skip ln=x\0 and command */
  int idx0, idx1, idx2;
  idx0 = packet.indexOf('\0');                    /* skip either ln or command */
  if (packet.startsWith("ln=")) {                 /* skip was on ln */
    int temp = packet.indexOf('\0', idx0 + 1);    /* skip command as well */
    ret["command"] = packet.mid(idx0 + 1, temp - idx0 - 1);
    idx0 = temp;
  } else {
    ret["command"] = packet.left(idx0);
  }
  idx1 = packet.indexOf('\0', idx0 + 1);          /* \0 after errorCode */
  
  /* errorCode[\1errorMessage] */         /* this is 'len' */
  QByteArray error = packet.mid(idx0 + 1, idx1 - idx0 - 1);
  
  /* now need to check for \1 */
  idx2 = error.indexOf("\1");                     /* a \1 indicates an errorMessage */
  
  if (idx2 == -1) { /* no errorMessage */
    ret["errorCode"] = error;
    ret["errorMessage"] = "";
  } else {
    ret["errorCode"] = error.left(idx2);
    ret["errorMessage"] = error.mid(idx2 + 1);
  }
  
  return ret;
}

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


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
static int packetError(const QByteArray &packet)
{
  /* == Example
   * 1\0
   * errorCode [\1errorMessage]\0
   * ... etc
   */
  QString error = packet.mid(packet.indexOf("\0"), packet.indexOf("\0", 1));
  
  /* now need to check for \1 */
  int idx1 = error.indexOf("\1");
  
  /* return errorCode only */
  return error.left(idx1 == -1 ? error.indexOf("\0") : idx1).toInt();
}

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


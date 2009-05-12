/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "33_blocksubscription.h"

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

/****************************************************************************
**
** Author: Marc Bowes
**
** Denies a subscription (friend) request permanently
**
****************************************************************************/
void BlockSubscription::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=33\0
  **  ms=contactAddress
  **
  ***************************************************************************
  */
  
  /* packet header setup */
  packet->setCommand("33");
  
  /* build packet data */
  (*packet) << variables["contactAddress"];
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Does nothing
**
****************************************************************************/
VariableHash BlockSubscription::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  33\0
  **  errorCode[\1errorMessage]\0
  **
  ***************************************************************************
  */
  
  return VariableHash();
}

}

}

}


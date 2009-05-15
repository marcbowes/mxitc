/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "56_unregistergateway.h"

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Populates a packet with the information required to unregister a gateway
**
****************************************************************************/
void UnregisterGateway::buildPacket(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=56
  **  ms=username \1 type
  **
  ***************************************************************************
  
  == DESCRIPTION
  ***************************************************************************
  **
  **  username            is the username of the existing IM gateway account
  **  type                is the type of the IM gateway according to the contact type
  **
  ***************************************************************************
  */
  
  (*packet) << variables["username"]
            << variables["type"];
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the unregister packet
**
****************************************************************************/
VariableHash UnregisterGateway::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  56\0
  **  errorCode[\1errorMessage]\0
  **
  ***************************************************************************
  
  */
  
  return VariableHash();
}

}

}

}

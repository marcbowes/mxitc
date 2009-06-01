/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "54_registergateway.h"

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
** Populates a packet with the information required to add other IM contacts
**
****************************************************************************/
void RegisterGateway::buildPacket(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=54
  **  ms=username \1 password \1 deprecated \1 type
  **
  ***************************************************************************
  
  == DESCRIPTION
  ***************************************************************************
  **
  **  username            is the username of the existing IM gateway account
  **  password            is the password of the existing IM gateway account
  **  deprecated          is deprecated functionality (should be empty string)
  **  type                the type of the IM gateway according to the contact type
  **                        0 - MXit
  **                        1 - Jabber
  **                        2 - MSN
  **                        3 - Yahoo
  **                        4 - ICQ
  **                        5 - AIM
  **                        8 - Bot
  **                        9 - Chat room
  **                        12 - Gallery
  **                        13 - Info
  **                        14 - MultiMx
  **                        18 - Google Talk
  **
  ***************************************************************************
  */
  
  (*packet) << variables["username"]
            << variables["password"]
            << "" /* deprecated */
            << variables["type"];
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Does bugger all
**
****************************************************************************/
VariableHash RegisterGateway::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  54\0
  **  errorCode[\1errorMessage]\0
  **
  ***************************************************************************
  
  */
  
  return VariableHash();
}

}

}

}

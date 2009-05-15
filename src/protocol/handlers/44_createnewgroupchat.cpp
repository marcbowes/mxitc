/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "44_createnewgroupchat.h"

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
** Populates a packet with the information required to create a new chatroom
**
****************************************************************************/
void CreateNewGroupchat::buildPacket(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname \0
  **  cm=44 \0
  **  ms=roomname \1 numContacts \1 contactAddress0 \1 ... \1 contactAddressN
  **
  ***************************************************************************
  */
  
  (*packet) << variables["roomname"]
            << variables["numContacts"]
            << variables["contactList"];
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the create new group chat packet
**
****************************************************************************/
VariableHash CreateNewGroupchat::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  44 \0
  **  errorCode [ \1 errorMessage ] \0
  **  roomid
  **
  ***************************************************************************
  
  == DEFINITIONS
  ***************************************************************************
  **
  **  roomid              an identifier that identifies this chatroom 
  **                      uniquely for each of its members.
  **
  ***************************************************************************
  
  */
  
  VariableHash variables;
  variables["roomid"] = packet;
  
  return variables;
}

}

}

}

/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "45_addnewgroupchatmember.h"

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
** Populates a packet with the information required to add a new member to a
** groupchat
**
****************************************************************************/
void AddNewGroupchatMember::buildPacket(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=45
  **  ms=roomid \1 numContacts \1 contactAddress0 \1 ... \1 contactAddressN
  **
  ***************************************************************************
  */
  
  /* no data */
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the add new group chat member
**
****************************************************************************/
VariableHash AddNewGroupchatMember::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  45\0
  **  errorCode[\1errorMessage]\0
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
  
  //FIXME: should we really send this back?
  VariableHash variables;
  variables["roomid"] = packet;
  
  return variables;
}

}

}

}

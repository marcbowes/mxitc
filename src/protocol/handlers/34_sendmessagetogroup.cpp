/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "34_sendmessagetogroup.h"

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
** Populates a packet with the information required to send a message to many
** contacts
**
****************************************************************************/
void SendMessageToGroup::buildPacket(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=34
  **  ms=group \1 numContacts \1 contactAddress0 \1 ... \1 contactAddressN \1
  **  msg \1 type \1 flags
  **
  ***************************************************************************
  
  == DESCRIPTION
  ***************************************************************************
  **
  **  group               is the new group
  **  numContacts         is the number of contacts affected by this command
  **  contactAddress0..N  is the list of contacts affected by this command
  **  msg                 the message that must be sent
  **  type                the message type
  **                        1 - Normal
  **                        2..N - Reserved
  **  flags               describes additional attributes of the message
  **                        0x10 - Encrypted with password
  **                        0x20 - Transport Layer Encryption (PID)
  **
  ***************************************************************************
  */
  
  (*packet) << variables["group"]
            << variables["numContacts"]
            << variables["contactList"]
            << variables["msg"]
            << variables["type"]
            << variables["flags"];
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the send message to group packet
**
****************************************************************************/
VariableHash SendMessageToGroup::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  34\0
  **  errorCode[\1errorMessage]\0
  **
  ***************************************************************************
  
  */
  
  return VariableHash();
}

}

}

}

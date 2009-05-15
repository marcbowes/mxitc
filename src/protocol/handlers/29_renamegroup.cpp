/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "29_renamegroup.h"

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
** Populates a packet with the information required to rename a group
**
****************************************************************************/
void RenameGroup::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=29
  **  ms=group \1 numContacts \1 contactAddress0 \1 alias0 \1 contactAddress1 \1 alias1 \1
  **  ... \1 contactAddressN \1 aliasN
  **
  ***************************************************************************
  
  == DESCRIPTION
  ***************************************************************************
  **
  **  group               is the new group name
  **  numContacts         is the number of contacts affected by this command
  **  contactAddress0..N  is the list of contacts affected by this command
  **  alias0..N           is the corresponding alias for each of the contacts affected
  **                      by this command
  **
  ***************************************************************************
  
  */
  
  /* packet header setup */
  packet->setCommand("29");
  
  (*packet) << variables["group"]
            << variables["numContacts"]
            << variables["contactList"];
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the rename group packet
**
****************************************************************************/
VariableHash RenameGroup::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  29\0
  **  errorCode[\1errorMessage]\0
  **
  ***************************************************************************
  
  */
  
  return VariableHash();
}

}

}

}

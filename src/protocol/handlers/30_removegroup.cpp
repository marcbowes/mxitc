/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "30_removegroup.h"

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
** Populates a packet with the information required to remove a group
**
****************************************************************************/
void RemoveGroup::buildPacket(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=30
  **  ms=deleteContacts \1 numContacts \1 contactAddress0 \1 alias0 \1 contactAddress1 \1
  **  alias1 \1 ... \1 contactAddressN \1 aliasN

  **
  ***************************************************************************
  
  == DESCRIPTION
  ***************************************************************************
  **
  **  deleteContacts      is a flag that indicates whether the contacts themselves should be
  **                      deleted.
  **                        0 - Do not delete the contacts (default)
  **                        1 - Delete all the specified contacts as well
  **  numContacts         is the number of contacts affected by this command
  **  contactAddress0..N  is the list of contacts affected by this command
  **  alias0..N           is the corresponding alias for each of the contacts affected
  **                      by this command
  **
  ***************************************************************************
  */
  
  (*packet) << variables["deleteContacts"]
            << variables["numContacts"]
            << variables["contactList"];
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the remove group packet
**
****************************************************************************/
VariableHash RemoveGroup::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  30\0
  **  errorCode[\1errorMessage]\0
  **
  ***************************************************************************
  
  */
  
  return VariableHash();
}

}

}

}

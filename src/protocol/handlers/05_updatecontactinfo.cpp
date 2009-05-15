/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "05_updatecontactinfo.h"

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
** Populates a packet with the information required to update a contacts
** information
**
****************************************************************************/
void UpdateContactInfo::buildPacket(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=5\0
  **  ms=group \1 contactAddress \1 nickname
  **
  ***************************************************************************
  
  == DEFINITIONS
  ***************************************************************************
  **
  **  group               is the new group the contact should be moved to
  **  contactAddress      identifies the contact to update
  **  nickname            is the contact's new nickname
  **
  ***************************************************************************
  */
  
  /* packet data setup */
  (*packet) << variables["group"]
            << variables["contactAddress"]
            << variables["nickname"]
  ;
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the update contact packet reply
**
****************************************************************************/
VariableHash UpdateContactInfo::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  5\0
  **  errorCode[\1errorMessage]
  **
  ***************************************************************************
  */
  
  return VariableHash();
}

}

}

}

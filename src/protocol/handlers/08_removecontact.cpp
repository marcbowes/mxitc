/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "08_removecontact.h"

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
** Populates a packet with the information required to remove a contact
**
****************************************************************************/
void RemoveContact::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=8\0
  **  ms=contactAddress
  **
  ***************************************************************************
  
  == DEFINITIONS
  ***************************************************************************
  **
  **  contactAddress      identifies the contact uniquely
  **
  ***************************************************************************
  */
  
  /* packet header setup */
  packet->setCommand("8");
  
  /* packet data setup */
  (*packet) << variables["contactAddress"];
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the remove contact packet reply
**
****************************************************************************/
VariableHash RemoveContact::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  8\0
  **  errorCode[\1errorMessage]
  **
  ***************************************************************************
  */
  
  return VariableHash();
}

}

}

}

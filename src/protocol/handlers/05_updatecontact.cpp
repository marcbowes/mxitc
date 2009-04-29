/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "05_updatecontact.h"

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
void UpdateContact::build(MXit::Network::Packet *packet, VariableHash &variables)
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
  **  deprecated          should be 0
  **
  ***************************************************************************
  */
  
  /* packet header setup */
  packet->setCommand("1");
  
  /* packet data setup */
  (*packet) << variables["group"];
  (*packet) << variables["contactAddress"];
  (*packet) << variables["nickname"];
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the update contact packet reply
**
****************************************************************************/
VariableHash UpdateContact::handle(const QByteArray &packet)
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
  
  
}

}

}

}

/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "06_addcontact.h"

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
** Populates a packet with the information required to request a contact
** addition
**
****************************************************************************/
void AddContact::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=6\0
  **  ms=group \1 contact_loginname \1 nickname \1 type \1 msg
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
  packet->setCommand("6");
  
  /* packet data setup */
  (*packet) << variables["group"];
  (*packet) << variables["contact_loginname"];
  (*packet) << variables["nickname"];
  (*packet) << variables["type"];
  (*packet) << variables["msg"];
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the add contact packet reply
**
****************************************************************************/
VariableHash AddContact::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  6\0
  **  errorCode[\1errorMessage]
  **
  ***************************************************************************
  */
  
  
}

}

}

}

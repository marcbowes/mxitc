/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "03_getcontacts.h"

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
** Populates a packet with the information required to request contacts
**
****************************************************************************/
void GetContacts::build(MXit::Network::Packet *packet, const VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  “id”=loginname [ \1 sesid ] \0
  **  “cm”=3
  **
  ***************************************************************************
  */
  
  /* packet header setup */
  packet->setCommand("1");
  packet->setCellphone(variables["_cellphone"]);
  
  /* no data */
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the logout packet
**
****************************************************************************/
void GetContacts::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  3\0
  **  errorCode [ \1 errorMessage ]
  **
  ***************************************************************************
  */
}

}

}

}

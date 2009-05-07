/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "02_logout.h"

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
** Populates a packet with the information required to logout
**
****************************************************************************/
void Logout::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=2\0
  **  ms=deprecated
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
  packet->setCommand("2");
  
  /* packet data setup */
  (*packet) << "0";
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the logout packet
**
****************************************************************************/
VariableHash Logout::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  2\0
  **  errorCode[\1errorMessage]
  **
  ***************************************************************************
  */
}

}

}

}

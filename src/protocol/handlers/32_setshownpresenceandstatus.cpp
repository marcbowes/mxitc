/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "32_setshownpresenceandstatus.h"

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

/****************************************************************************
**
** Author: Marc Bowes
**
** Populates a packet with the information for setting a presence
**
****************************************************************************/
void SetShownPresenceAndStatus::buildPacket(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname [\1sesid]\0
  **  cm=32\0
  **  ms=show\1status
  **
  ***************************************************************************
  
   == DEFINITIONS
  ***************************************************************************
  **
  **  show[1]             indicates the availability of the user
  **  status              a natural language description of presence status
  **                      to provide detailed description of an availability
  **                      state
  **
  ***************************************************************************
  
  == NOTES
  ***************************************************************************
  **
  **  [1] Show
  **    1                 online
  **    2                 away
  **    4                 dnd (do not disturb)
  **
  ***************************************************************************
  */
  
  (*packet) << variables["show"]
            << variables["status"]
  ;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Does nothing (error handling at client level)
**
****************************************************************************/
VariableHash SetShownPresenceAndStatus::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  10\0
  **  errorCode[\1errorMessage]
  **
  ***************************************************************************
  */
  
  return VariableHash();
}

}

}

}


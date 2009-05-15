/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "41_setmood.h"

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
** Populates a packet with the information required to set the users mood
**
****************************************************************************/
void SetMood::buildPacket(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=41
  **  ms=mood
  **
  ***************************************************************************
  
  == DEFINITIONS
  ***************************************************************************
  **
  **  mood specifies the user's mood. Empty implies no mood.
  **    0 - no mood
  **    1 - angry
  **    2 - excited
  **    3 - grumpy
  **    4 - happy
  **    5 - in love
  **    6 - invincible
  **    7 - sad
  **    8 - hot
  **    9 - sick
  **    10 - sleepy
  ***************************************************************************
  */
  
  /* no data */
  (*packet) << variables["mood"];
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the set mood packet
**
****************************************************************************/
VariableHash SetMood::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  41\0
  **  errorCode[\1errorMessage]
  **
  ***************************************************************************
  
  */
  
  return VariableHash();
}

}

}

}

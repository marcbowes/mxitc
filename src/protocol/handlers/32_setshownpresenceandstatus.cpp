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
void SetShownPresenceAndStatus::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  // stub
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
  // stub
}

}

}

}


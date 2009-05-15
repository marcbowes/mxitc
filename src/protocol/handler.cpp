/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "handler.h"

/* this is really just so that the .o file is built */

namespace MXit
{

namespace Protocol
{

/****************************************************************************
**
** Author: Marc Bowes
**
** "Before filter" to set the Packet command
**
****************************************************************************/
void Handler::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  packet->setCommand(QString("%1").arg(command));
  buildPacket(packet, variables);
}

}

}


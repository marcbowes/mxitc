/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "43_loginkick.h"

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
** Stub
**
****************************************************************************/
void LoginKick::buildPacket(MXit::Network::Packet *packet, VariableHash &variables)
{
  /* nothing */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Does nothing
**
****************************************************************************/
VariableHash LoginKick::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  43\0
  **  errorCode[\1errorMessage]\0
  **
  ***************************************************************************
  */
  
  return VariableHash();
}

}

}

}


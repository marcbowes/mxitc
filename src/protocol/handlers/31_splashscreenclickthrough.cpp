/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "31_splashscreenclickthrough.h"

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
** Sends through the name of the custom resource chunk (see #27)
**
****************************************************************************/
void SplashScreenClickthrough::buildPacket(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=31\0
  **  ms=name
  **
  ***************************************************************************
  
  == DESCRIPTION
  ***************************************************************************
  **
  **  name                name of splash screen (see #27)
  **
  ***************************************************************************
  */
  
  (*packet) << variables["name"];
}

/****************************************************************************
**
** Author: Marc Bowes
**
** Nothing
**
****************************************************************************/
VariableHash SplashScreenClickthrough::handle(const QByteArray &packet)
{  
  return VariableHash();
}

}

}

}

/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "1k_keepalive.h"

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
** Empty command to prevent a login-kick (#43)
**
****************************************************************************/
void KeepAlive::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=1000
  **
  ***************************************************************************
  */
  
  /* packet header setup */
  packet->setCommand("1000");
  
  /* no data */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Does nothing
**
****************************************************************************/
VariableHash KeepAlive::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  1000\0
  **  errorCode[\1errorMessage]\0
  **
  ***************************************************************************
  */
  
  return VariableHash();
}

}

}

}


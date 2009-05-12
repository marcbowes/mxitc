/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "52_allowsubscription.h"

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
** allows a subscription (friend) request and assigns a handle
**
****************************************************************************/
void AllowSubscription::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=52
  **  ms=contactAddress[\1group\1nickname]
  **
  ***************************************************************************
  */
  
  /* packet header setup */
  packet->setCommand("52");
  
  /* build packet data */
  (*packet) << variables["contactAddress"]
            << variables["group"]
            << variables["nickname"]
  ;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Does nothing
**
****************************************************************************/
VariableHash AllowSubscription::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  52\0
  **  errorCode[\1errorMessage]\0
  **
  ***************************************************************************
  */
  
  return VariableHash();
}

}

}

}


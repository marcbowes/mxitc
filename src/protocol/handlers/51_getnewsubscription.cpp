/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "51_getnewsubscription.h"

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
** stub
**
****************************************************************************/
void GetNewSubscription::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=51
  **
  ***************************************************************************
  */
  
  /* packet header setup */
  packet->setCommand("51");
  
  /* no data */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Extracts variable information from the subscription request packet
**
****************************************************************************/
VariableHash GetNewSubscription::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  51\0
  **  errorCode[\1errorMessage]\0
  **  contactAddress0\1nickname0\1type0\1hiddenLoginname0\1msg0\1groupchatmod0\0
  **  ...
  **  contactAddressN\1nicknameN\1typeN\1hiddenLoginnameN\1msgN\1groupchatmodN
  **
  ***************************************************************************
  
  == DEFINITIONS
  ***************************************************************************
  **
  ** contactAddress       identifies the contact that sent the subscription
  **                      request
  ** nickname             is a nickname for that contact
  ** type                 the type of contact this subscription request is
  **                      intended for
  ** hiddenLoginname      indicates if the contact's loginname should be hidden
  **                      in the client:
  **                      0 - not hidden
  **                      1 - hidden
  ** msg                  a custom message for this contact
  ** groupchatmod         the name of the creator of the MultiMx
  **
  ***************************************************************************
  */
  
  VariableHash variables;
  variables["contacts"] = packet;
  
  return variables;
}

}

}

}


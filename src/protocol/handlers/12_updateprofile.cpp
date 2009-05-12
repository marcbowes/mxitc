/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "12_updateprofile.h"

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
** Populates a packet with the information required to update a contacts
** information
**
****************************************************************************/
void UpdateProfile::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  ** id=loginname[\1sesid]\0
  ** cm=12\0
  ** ms=pin\1name[\1hiddenLoginname\1yearOfBirth\1gender\1deprecated]
  **
  ***************************************************************************
  
  == DEFINITIONS
  ***************************************************************************
  **
  ** pin                  is the user's new password
  ** name                 is the new name/nickname of the user
  ** hiddenLoginname      specifies whether the user's loginname should be
  **                      hidden when invite a contact:
  **                      0 = NOT hidden, 1 = hidden
  ** yearOfBirth          an ISO 8601 data of the client's birth (YYYY-MM-DD)
  **
  ***************************************************************************
  */
  
  /* packet header setup */
  packet->setCommand("12");
  
  /* packet data setup */
  (*packet) << variables["pin"]
            << variables["name"]
            << variables["hiddenLoginname"]
            << variables["yearOfBirth"]
            << variables["gender"]
  ;       /* FIXME: deprecated? */
}

/****************************************************************************
**
** Author: Marc Bowes
**
** Does nothing
**
****************************************************************************/
VariableHash UpdateProfile::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  12\0
  **  errorCode[\1errorMessage]
  **
  ***************************************************************************
  */
  
  return VariableHash();
}

}

}

}


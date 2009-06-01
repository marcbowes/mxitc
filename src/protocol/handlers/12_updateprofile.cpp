/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "protocol/aes.h"

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
void UpdateProfile::buildPacket(MXit::Network::Packet *packet, VariableHash &variables)
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
  
  /* first - password encryption (Appendix B) */
  if (!variables.value("pin").isEmpty()) {
    variables.remove("encryptedpassword");
    QString key = QString("6170383452343567").replace(0, 8, variables["pid"].right(8));
    QString pass = "<mxit/>" + variables["pin"];
    MXit::Protocol::AES encryptor;
    variables["encryptedpassword"] = encryptor.encrypt(key.toLatin1(), pass.toLatin1()).toBase64();
  }
  
  /* packet data setup */
  (*packet) << variables["encryptedpassword"]
            << variables["name"]
            << variables["hiddenLoginname"]
            << variables["yearOfBirth"]
            << variables["gender"]
            << "" /* deprecated */
  ;
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


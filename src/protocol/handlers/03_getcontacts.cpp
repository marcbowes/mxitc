/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "03_getcontacts.h"

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
** Populates a packet with the information required to request contacts
**
****************************************************************************/
void GetContacts::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=3
  **
  ***************************************************************************
  */
  
  /* packet header setup */
  packet->setCommand("3");
  
  /* no data */
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the logout packet
**
****************************************************************************/
void GetContacts::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  3\0
  **  errorCode[\1errorMessage]\0
  **  group0[\1contactAddress0\1nickname0\1presence0\1type0\1mood\0
  **  ..
  **  groupN[\1contactAddressN\1nicknameN\1presenceN\1typeN\1mood
  **
  ***************************************************************************
  */
  
  int i, count = 0;
  for (i=0; (count < 1)||(packet.at(i) != '\0'); i++) {
    if (packet.at(i) == '\0')
      count++;
    //FIXME: remove when proven to be working
    if (i > packet.size())
      qDebug() << "AH FUCK!";
  }
  
  
}

}

}

}

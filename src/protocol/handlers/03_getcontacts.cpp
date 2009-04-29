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
** Extracts variable information from the get contacts packet
**
****************************************************************************/
VariableHash GetContacts::handle(const QByteArray &packet)
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
  
  == DEFINITIONS
  ***************************************************************************
  **
  **  group               specifies the group the contact belongs to (maximum
  **                      length = 32 characters)
  **  contactAddress      identifies the contact uniquely (maximum length =  
  **                      64 characters)
  **  nickname            a nickname the user wants displayed for this 
  **                      contact instead of the full jid (maximum length = 
  **                      48 characters)
  **  presence            specifies the state of the contact (see below)
  **  type                specifies the type of the contact (see below)
  **  mood                specifies the mood of the contact (see below)
  **
  ***************************************************************************
  
  == NOTES
  ***************************************************************************
  **  
  **  Presence:
  **    The following presence states are defined:
  **      0 - offline
  **      1 - online
  **      2 - away (but still online)
  **      4 - do not disturb
  **  
  **  Type:
  **    The following contact types are defined:
  **      0 - MXit
  **      1 - Jabber
  **      2 - MSN
  **      3 - Yahoo
  **      4 - ICQ
  **      5 - AIM
  **      8 - Bot
  **      9 - Chat room
  **      12 - Gallery
  **      13 - Info
  **      14 - MultiMx
  **      18 - Google Talk
  **  
  **  Mood:
  **    The following moods are defined:
  **      0 - None
  **      1 - Angry
  **      2 - Excited
  **      3 - Grumpy
  **      4 - Happy
  **      5 - In love
  **      6 - Invincible
  **      7 - Sad
  **      8 - Hot
  **      9 - Sick
  **      10 - Sleepy
  **
  ***************************************************************************
  
  */
  
  int i, count = 0;
  for (i=0; (count < 4)||(packet.at(i) != '\0'); i++) {
    if (packet.at(i) == '\0')
      count++;
    //FIXME: remove when proven to be working
    if (i > packet.size())
      qDebug() << "AH FUCK!";
  }
  
  QByteArray contactData = packet.right(packet.size() - i);
  //FIXME: remove when proven to be working
  if (contactData.at(0) == '\0')
    qDebug() << "l2count noob!";
  
  VariableHash variable;
  variable["contacts"] = contactData;
  
  return variable;
}

}

}

}

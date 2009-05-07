/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "09_getnewmessages.h"

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
** Populates a packet with the information required to request new messages
**
****************************************************************************/
void GetNewMessages::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname [ \1 sesid ] \0
  **  cm=9
  **
  ***************************************************************************
  
  */
  
  /* packet header setup */
  packet->setCommand("9");
  
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the get messages packet
**
****************************************************************************/
VariableHash GetNewMessages::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  9 \0
  **  errorCode [ \1 errorMessage ] \0
  **  contactAddress \1 dateTime \1 type [ \1 id \1 flags ] \0
  **  msg
  **
  ***************************************************************************
  
   == DEFINITIONS
  ***************************************************************************
  **
  **  contactAddress      identifies the contact from whom the message was 
  **                      sent
  **  dateTime            the date and time in number of seconds since 1 
  **                      January 1970
  **  type                the message type (see below)
  **  id                  identifies the message uniquely for this session
  **  flags               specifies message properties (see below)
  **  msg                 the actual message
  **
  ***************************************************************************

  == NOTES
  ***************************************************************************
  **  
  **  Type:
  **    The following types are defined:
  **      1 - Normal
  **      2 - Chat
  **      3 - Headline
  **      4 - Error
  **      5 - Groupchat
  **      6 - MXit Custom Form
  **      7 - MXit Command
  **  
  **  Flags:
  **    The following flags are defined:
  **      0x2             Request delivery notification (only available to 
  **                      services)
  **      0x4             Request read notification, i.e. only when message 
  **                      if displayed on screen notification should be sent.
  **      0x10            Password encrypted
  **      0x20            Transport encrypted
  **      0x40            Reply should be sent password encrypted
  **      0x80            Reply should be sent transport encrypted
  **      0x200           Message may contain markup.
  **      0x400           Indicates that this message contains custom emoticons (see Appendix F).
  **
  ***************************************************************************

  */
  
  /* setup */
  StringVec variables;
  
  /* first break up packet by \0 into variable sections */
  variables.append("ln");                   /* ln=X\0 */
  variables.append("command");              /* 8\0 */
  variables.append("error");                /* errorCode[\1errorMessage]\0 */
  variables.append("contactData");          /* contactAddress \1 dateTime \1 type [ \1 id \1 flags ]\0 */
  variables.append("message");              /* msg */
  
  /* extract \0 seperated values */
  VariableHash rawMessage = hashVariables(packet, variables, '\0');
  
  variables.clear();
  variables.append("contactAddress");
  variables.append("dateTime");
  variables.append("type");
  variables.append("id");
  variables.append("flags");
  
  VariableHash contactDetails = hashVariables(rawMessage["contactData"], variables, '\1');
  
  return rawMessage.unite(contactDetails);
}

}

}

}

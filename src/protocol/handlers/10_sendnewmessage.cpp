/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "10_sendnewmessage.h"

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
** Populates a packet with the information required to send a message
**
****************************************************************************/
void SendNewMessage::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=10\0
  **  ms=contactAddress\1msg\1type\1flags
  **
  ***************************************************************************
  
  == DEFINITIONS
  ***************************************************************************
  **
  **  contactAddress      indentifies the contact uniquely
  **  msg                 the message that must be sent
  **  type                the message type (see protocol/message_type.h)
  **  flags[1]            Specifies additional properties of the message
  **
  ***************************************************************************
  
  == NOTES
  ***************************************************************************
  **
  **  [1] Flags
  **    0x01              Password encrypted
  **    0x20              Transport encrypted
  **    0x200             Message may contain markup
  **
  ***************************************************************************
  */
  
  /* packet header setup */
  packet->setCommand("10");
  
  /* packet data setup */
  (*packet) << variables["contactAddress"]
            << variables["msg"]
            << variables["type"]
            << variables["flags"]
  ;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Does nothing (error handling at client level)
**
****************************************************************************/
VariableHash SendNewMessage::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=10\0
  **  ms=contactAddress\1msg\1type\1flags
  **
  ***************************************************************************
  
  == DEFINITIONS
  ***************************************************************************
  **
  **  contactAddress      indentifies the contact uniquely
  **  msg                 the message that must be sent
  **  type                the message type (see protocol/message_type.h)
  **  flags[1]            Specifies additional properties of the message
  **
  ***************************************************************************
  
  == NOTES
  ***************************************************************************
  **
  **  [1] Flags
  **    0x01              Password encrypted
  **    0x20              Transport encrypted
  **    0x200             Message may contain markup
  **
  ***************************************************************************
  */
  
  /* setup */
  StringVec variables;
  
  /* first break up packet by \0 into variable sections */
  variables.append("ln");                   /* ln=X\0 */
  variables.append("command");              /* 10\0 */
  variables.append("ms");
  
  /* extract \0 seperated values */
  VariableHash pass1 = hashVariables(packet, variables, '\0');
  pass1.remove("command");                  /* we know this is 1 */
  
  /* need to expand data section */
  variables.clear();
  variables.append("contactAddress");
  variables.append("message");
  variables.append("type");
  variables.append("flag");
  
  /* extract \1 seperated values */
  VariableHash pass2 = hashVariables(pass1["ms"], variables, '\1');
  
  return pass1.unite(pass2);
}

}

}

}


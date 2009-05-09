/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "11_register.h"

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
** Populates a packet with the information required to register a new account
**
****************************************************************************/
void Register::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=11\0
  **  ms=password \1 version \1 maxReplyLen [ \1 name ] \1 dateOfBirth \1 
  **  gender \1 location \1 capabilities \1 dc \1 features \1 dialingCode \1
  **  locale \0 FIXME: find out if this actually should be null terminated
  **
  ***************************************************************************
  
  == DEFINITIONS
  ***************************************************************************
  **
  **  password            is the 7 to 10 digit pin (ONLY numeric characters 
  **                      allowed)
  **  version             the version number (see 1. Login)
  **  maxReplyLen         the maximum length a single reply may be
  **  name                the user's name/nickname
  **  dateOfBirth         an ISO 8601 date of the user’s birth (format 
  **                      YYYY-MM-DD)
  **  gender              the user's gender:
  **                        0 - female
  **                        1 - male
  **  location            a string describing the user’s location (max 64 
  **                      characters)
  **  capabilities        see Login command
  **  dc                  distribution code that identifies the 
  **                      loginname/installation combination
  **                      uniquely
  **  features            a bitset describing client features (see 1. Login)
  **  dialingCode         the international dialing code for the user's country (see Appendix A)
  **  locale              the user’s locale (see Appendix A)
  **
  ***************************************************************************
  */
  
  /* packet header setup */
  packet->setCommand("11");
  
  /* packet data setup */
  (*packet) << "0";
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the register packet
**
****************************************************************************/
VariableHash Register::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  11 \0
  **  errorCode [ \1 errorMessage ] \0
  **  sesid \0
  **  deprecated \1 loginname \1 timeStamp \1 serverIP \1 maxSupportedVer \1 pricePlan \1
  **  flags \0
  **  hiddenLoginname
  **  [ \0 Poll data ]
  **
  ***************************************************************************
  
  == DEFINITIONS
  ***************************************************************************
  **
  **  errorCode           see 1. Login
  **  deprecated          deprecated functionality (expect an empty string)
  **  loginname           is the user’s loginname
  **  timeStamp           the number of seconds since 1 January 1970 (UTC)
  **  serverIP            the IP address of the server
  **  maxSupportedVer     maximum protocol version supported by the server
  **                      (major*10+minor)
  **  pricePlan           the price plan the user is on:
  **                        1 – free
  **                        2 – premium
  **  flags               specific flags for this user
  **  hiddenLoginname     specifies whether the user’s loginname should be 
  **                      hidden when inviting a contact:
  **                        0 – not hidden
  **                        1 – hidden
  **  poll data           this will contain the user’s contacts as well as 
  **                      their presence information and all new messages.
  **
  ***************************************************************************
  */
  
  return VariableHash();
}

}

}

}

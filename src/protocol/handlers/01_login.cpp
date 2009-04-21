/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "01_login.h"

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
** Populates a packet with the information required to login
**
****************************************************************************/
void Login::build(MXit::Network::Packet *packet, const VariableHash &variables)
{
  // ..
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Extracts variable information from the login packet
**
****************************************************************************/
void handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  1\0
  **  errorCode [\1errorMessage]\0
  **  sesid\0
  **  deprecated\1loginname\1dateTime\1URL\1
  **  maxSupportedVer\1pricePlan\1flags\0
  **  [\0Poll data]
  **
  ***************************************************************************
  
  == DEFINITIONS
  ***************************************************************************
  **
  **  sesid               the session ID (>1 for HTTP connections and 0 for
  **                      TCP connections)
  **  deprecated          deprecated functionality (expect an empty string)
  **  loginname           the user's loginname
  **  dateTime            the date and time in number of seconds since
  **                      1 January 1970 (UTC)
  **  URL                 is the URL of the proxy handling your current
  **                      session. Use this URL to reconnect to if
  **                      disconnected.
  **  maxSupportedVer     maximum protocol version supported by the server
  **                      (major*10+minor)
  **  pricePlan           the price plan the user is on:
  **                      1 - free
  **                      2 - premium
  **  flags               contains specific flags for this user. At the
  **                      moment only indicates if session is encrypted or not.
  **  poll data           if getContacts was set to 1, this will contain the
  **                      user's contacts as well as their presence information
  **                      and all new messages;
  **
  ***************************************************************************
  
  == ERRORS
  ***************************************************************************
  **
  **  3                   Invalid password
  **  16                  Redirect[1] to new proxy
  **  99                  Something went wrong
  **
  ***************************************************************************
  
  == NOTES
  ***************************************************************************
  **
  **  [1] Redirect
  **    The client is requested to redirect to the URL specified in the
  **    errorMessage field. The URL will be in the following format:
  **    protocol://host:port;type[;msg]
  **
  ***************************************************************************
  */
}

}

}

}


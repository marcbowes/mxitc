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
** FIXME: Poll data looks fishy
**
****************************************************************************/
void Login::handle(const QByteArray &packet)
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
  
  /* setup */
  StringVec variables;
  
  /* error checking */
  int errorCode = packetError(packet);
  if (errorCode != 0) {                     /* No error */
    // FIXME: how to do proper error reporting?
    
    return;
  }
  
  /* continue with error-free reply */
  
  /* first break up packet by \0 into variable sections */
  variables.append("command");              /* 1\0 */
  variables.append("error");                /* errorCode[\1errorMessage]\0 */
  variables.append("sesid");                /* sesid\0 */
  variables.append("data");                 /* deprecated..flags\0 */
  variables.append("poll data");            /* [\0Poll data] */
  
  /* extract \0 seperated values */
  VariableHash pass1 = hashVariables(packet, variables, "\0");
  pass1.remove("command");                  /* we know this is 1 */
  pass1.remove("error");                    /* no error, handled earlier */
  
  /* need to expand data section */
  variables.clear();
  variables.append("deprecated");
  variables.append("loginname");
  variables.append("dateTime");
  variables.append("URL");
  variables.append("maxSuppertedVer");
  variables.append("pricePlan");
  variables.append("flags");
  
  /* extract \1 seperated values */
  VariableHash pass2 = hashVariables(pass1["data"], variables, "\1");
  
  /* no clean-up needed, just return the variables */
  emit outgoingVariables(pass1.unite(pass2));
}

}

}

}


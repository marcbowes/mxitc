/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "protocol/aes.h"

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
** Author: Richard Baxter
** Author: Tim Sjoberg
**
** Populates a packet with the information required to login
**
****************************************************************************/
void Login::buildPacket(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname\0
  **  cm=1\0
  **  ms=password\1version\1getContacts\1capabilities\1
  **     dc\1features\1dialingCode\1locale\0
  **  [cr=splashName0\1splashName1\1...\1splashNameN]
  **
  ***************************************************************************
  
  == DEFINITIONS
  ***************************************************************************
  **
  **  password            The user's password (4-10 digits). Appendix B.
  **  version[1]          distributorCode-releaseVersion-archSeries-platform
  **  getContacts         0 - just logs in and does not return contacts
  **                      1 - returns contacts, presence and messages after
  **                      login
  **  capabilities[2]     Set of name-value pairs describing capabilities of
  **                      the software
  **  dc                  Distribution code - Identifies the installation/
  **                      loginname combination
  **  features[3]         Bitset describing features supported by the client
  **  dialingCode         International dialing code for the user. Append A.
  **  locale              Locale for the user. Appendix A.
  **
  ***************************************************************************
  
  == NOTES
  ***************************************************************************
  **
  **  [1] Version
  **    distributorCode-releaseVersion-archSeries-platform
  **
  **   -distributorCode identifies the distributor of the software.
  **   -releaseVersion identifies the software release in MajorNr.MinorNr.Build
  **    format. All numbers making up the release version should be smaller
  **    than 10.
  **   -archSeries is a rough guide of the mobile series supported by the
  **    software. Current values are:
  **      0       Generic MIDP 1.0 not fullscreen (128x160) - halfduplex
  **      ..elided (pg 7)..
  **      Y       PC Client (200x230)
  **   -Platform gives more information about the platform the software is
  **    currently running on.
  **
  **    e.g. "E-5.1.1-C-Nokia3100/05.54" for
  **          Nokia 3100 with version 5.1.1 software
  **
  **  [2] Capabilities
  **    The following capabilities are defined:
  **      w       Screen width in pixels
  **      h       Screen height in pixels
  **      ..elided (pg 8)..
  **
  **    e.g. w=176;h=220;c=65536;utf8=true
  **
  **  [3] Features
  **    Available features:
  **      Forms   0x01
  **      ..elided (pg 8)..
  **
  ***************************************************************************
  */
  
  /* packet data setup */
  
  /* first - password encryption (Appendix B) */
  if (variables.value("encryptedpassword").isEmpty()) {
    QString key = QString("6170383452343567").replace(0, 8, variables["pid"].right(8));
    QString pass = "<mxit/>" + variables["_password"];
    MXit::Protocol::AES encryptor;
    variables["encryptedpassword"] = encryptor.encrypt(key.toLatin1(), pass.toLatin1()).toBase64();
  }
  
  /* next - get distributor code from pid */
  if (variables.value("dc").isEmpty()) {
    QByteArray dc = variables["pid"];
    dc.replace(0, 2, "");
    dc = dc.left(dc.length() - 8);
    variables["dc"] = dc;
  }
  
  /* write data to packet */  
  (*packet) << variables["encryptedpassword"]
            << "E-5.8.2-Y-LPM"                    // version
            << "1"                                // getcontacts
            << "utf8=false;ctyp=8129"             // capabilities
            << variables["dc"]                    // dc
            << "524287"                           // features
            << variables["defaultDialingCode"]    // dialingCode
            << variables["locale"]                // locale
  ;
  
  QByteArray postMs;
  postMs.append('\0');
  postMs.append("cr=");
  
  packet->setPostMs(postMs);
}


/****************************************************************************
**
** Author: Marc Bowes
** Author: Tim Sjoberg
**
** Extracts variable information from the login packet
** FIXME: Poll data looks fishy
**
****************************************************************************/
VariableHash Login::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  1\0
  **  errorCode[\1errorMessage]\0
  **  sesid\0
  **  deprecated\1loginname\1dateTime\1URL\1
  **  maxSupportedVer\1pricePlan\1flags
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
  
  /* first break up packet by \0 into variable sections */
  variables.append("sesid");                /* sesid\0 */
  variables.append("data");                 /* deprecated..flags\0 */
  
  /* extract \0 seperated values */
  VariableHash pass1 = hashVariables(packet, variables, '\0');

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
  VariableHash pass2 = hashVariables(pass1["data"], variables, '\1');
  
  /* no clean-up needed, just return the variables */
  return pass1.unite(pass2);
}

}

}

}


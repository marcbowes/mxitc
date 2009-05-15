/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "packet_header.h"

namespace MXit
{

namespace Protocol
{

/****************************************************************************
**
** Author: Marc Bowes
**
** extracts the error code from a response
**
****************************************************************************/
VariableHash packetHeader(QByteArray &packet)
{
  /* == Example
   * ln=x\0                           TCP only
   * 1\0
   * errorCode[\1errorMessage]\0
   * ... etc
   */
  VariableHash ret;
  
  /* skip ln=x\0 and command */
  int idx0, idx1, idx2;
  idx0 = packet.indexOf('\0');                    /* skip either ln or command */
  if (packet.startsWith("ln=")) {                 /* skip was on ln */
    int temp = packet.indexOf('\0', idx0 + 1);    /* skip command as well */
    ret["command"] = packet.mid(idx0 + 1, temp - idx0 - 1);
    idx0 = temp;
  } else {
    ret["command"] = packet.left(idx0);
  }
  idx1 = packet.indexOf('\0', idx0 + 1);          /* \0 after errorCode */
  
  /* errorCode[\1errorMessage] */         /* this is 'len' */
  QByteArray error = packet.mid(idx0 + 1, idx1 - idx0 - 1);
  
  /* now need to check for \1 */
  idx2 = error.indexOf("\1");                     /* a \1 indicates an errorMessage */
  
  if (idx2 == -1) { /* no errorMessage */
    ret["errorCode"] = error;
    ret["errorMessage"] = "";
  } else {
    ret["errorCode"] = error.left(idx2);
    ret["errorMessage"] = error.mid(idx2 + 1);
  }
  
  /* now strip the header information out the packet (don't need it) */
  packet.remove(0, idx1 > idx2 ? idx1 : idx2);
  
  return ret;
}

}

}


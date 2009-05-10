/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "17_polldifference.h"

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
** Populates a packet with the information request new data on an http 
** connection
**
****************************************************************************/
void PollDifference::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname [ \1 sesid ] \0 (sesid probably shouldn't be in brackets 
  **                                since this is http only)
  **  cm=17
  **
  ***************************************************************************
  */
  
  /* packet header setup */
  packet->setCommand("17");
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the poll difference packet
**
****************************************************************************/
VariableHash PollDifference::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  17 \0
  **  errorCode [ \1 errorMessage ] \0
  **  The (difference) results of the following 4 calls in the following order:
  **       get all contacts (3)
  **       get new messages (9)
  **       get subscription requests (51)
  **       get multimedia message (27), chunk 6
  **
  ***************************************************************************
  */
  
  return VariableHash();
}

}

}

}

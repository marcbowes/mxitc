/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "27_getmultimediamessage.h"

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
** Populates a packet with the information required to request binary data
**
****************************************************************************/
void GetMultimediaMessage::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=27\0
  **  ms=chunkedData
  **
  ***************************************************************************
  
  == DEFINITIONS
  ***************************************************************************
  **
  **  chunkedData         see appendix G. too long to paste
  **
  ***************************************************************************
  */
  
  /* packet header setup */
  packet->setCommand("27");
  
  /* packet data setup */
  (*packet) << "oogelyboogely";//FIXME: not a real request :)
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts nothing at the moment
**
****************************************************************************/
VariableHash GetMultimediaMessage::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  27\0
  **  errorCode[\1errorMessage]\0
  **  chunkedData
  **
  ***************************************************************************
  
  == DEFINITIONS
  ***************************************************************************
  **
  **  chunkedData         see appendix G. too long to paste
  **
  ***************************************************************************
  */
  
  return VariableHash();
}

}

}

}

/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "26_getprofile.h"

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
void GetProfile::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=26
  **
  ***************************************************************************
  */
  
  /* packet header setup */
  packet->setCommand("26");
  
  /* no data */
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts variable information from the get contacts packet
**
****************************************************************************/
VariableHash GetProfile::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  26\0
  **  errorCode[\1errorMessage]\0
  **  name \1 hiddenLoginname \1 yearOfBirth \1 gender
  **
  ***************************************************************************
  */  
  
  StringVec variables;
  
  variables.append("name");
  variables.append("hiddenLoginname");
  variables.append("yearOfBirth");
  variables.append("gender");
  
  VariableHash profileData = hashVariables(packet, variables, '\1');
  
  return VariableHash();
}

}

}

}

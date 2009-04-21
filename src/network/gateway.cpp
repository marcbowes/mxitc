/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "gateway.h"

namespace MXit
{

namespace Network
{

/****************************************************************************
**
** Author: Marc Bowes
**
** Gateway constructor
**
****************************************************************************/
Gateway::Gateway(const QString &gateway)
{
  build(gateway);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Gateway constructor
**
****************************************************************************/
Gateway::Gateway()
{
  // nothing
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Gateway deconstructor
**
****************************************************************************/
Gateway::~Gateway()
{
  // nothing
}

/****************************************************************************
**
** Author: Marc Bowes
**
** builds the Gateway from a connection string
**
****************************************************************************/
void Gateway::build(const QString &gateway)
{
  // TODO
}

}

}


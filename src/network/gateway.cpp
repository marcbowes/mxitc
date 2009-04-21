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
  /* either
   * http://$1                $1 = URL      OR
   * socket://$1:$2           $1 = host, $2 = port
   */
  if (gateway.startsWith("http://", Qt::CaseInsensitive)) {
    type = HTTP;
    
    URL = gateway.mid(7); /* http:// = 7 */
  } else { /* FIXME: just assuming TCP */
    type = TCP;
    
    QString host_colon_port = gateway.mid(9); /* socket:// = 9 */
    int colon = host_colon_port.indexOf(":");
    
    host = host_colon_port.left(colon);
    port = host_colon_port.mid(colon + 1).toInt();
  }
}

}

}


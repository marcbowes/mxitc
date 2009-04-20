/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Network::Connection implements a client connection to MXit via
** either TCP or HTTP
**
****************************************************************************/

#ifndef __MXIT_NETWORK_CONNECTION_H__
#define __MXIT_NETWORK_CONNECTION_H__

#include <QObject>

#include "protocol/commands.h"
#include "protocol/error_codes.h"

namespace MXit
{

namespace Network
{

class Connection : public QObject
{
  Q_OBJECT

  public:         /* class specific */
	
  Connection();
  ~Connection();

  signals:
  
  // ..
  
  private slots:
  
  // ..

  public:         /* methods */
  
  // ..

  private:        /* methods */
  
  // ..

  private:        /* variables */
  
  // ..
};

}

}

#endif


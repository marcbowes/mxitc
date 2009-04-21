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

#include <QMutex>
#include <QTcpSocket>
#include <QThread>
#include <QWaitCondition>

#include "common/types.h"

#include "protocol/commands.h"
#include "protocol/error_codes.h"

#include "gateway.h"
#include "http_packet.h"
#include "tcp_packet.h"
#include "packet.h"

namespace MXit
{

namespace Network
{

typedef QVector         <Gateway>  GatewayVec;
typedef QVectorIterator <Gateway>  GatewayVecItr;

class Connection : public QThread
{
  Q_OBJECT

  public:         /* class specific */
	
  Connection();
  ~Connection();

  signals:
  
  void error(QTcpSocket::SocketError error);

  private slots:
  
  // ..

  public:         /* methods */
  
  void    addGateway(const QString &connectionString);
  Packet *buildPacket();
  void    enqueue(const Packet &packet);
  void    run();

  private:        /* methods */
  
  // ..
  
  public:         /* variables */
  
  Gateway         gateway;
  
  private:        /* variables */
  
  GatewayVec      gateways;
  ByteArrayVec    queue;
  QMutex          queueMutex;
  QWaitCondition  queueWait;
  QTcpSocket     *socket;       /* TCP only */

};

}

}

#endif


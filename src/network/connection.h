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

#include <QTcpSocket>

#include "common/types.h"

#include "protocol/error_codes.h"

#include "gateway.h"
#include "packets/http.h"
#include "packets/tcp.h"
#include "packet.h"

namespace MXit
{

namespace Network
{

typedef QVector         <Gateway>  GatewayVec;
typedef QVectorIterator <Gateway>  GatewayVecItr;

class Connection : public QObject
{
  Q_OBJECT

  public:         /* class specific */
	
  Connection();
  ~Connection();
  
  enum State {
    CONNECTED,
    CONNECTING,
    DISCONNECTED
  };

  signals:
  
  void TCP_error(QTcpSocket::SocketError error);
  void outgoingError(const QString &message);
  void outgoingPacket(const QByteArray &packet);

  private slots:
  
  void incomingPacket();
  void TCP_connect();
  void TCP_connected();
  void TCP_disconnect();
  void TCP_disconnected();
  void TCP_read();

  public:         /* methods */
  
  void addGateway(const QString &connectionString);
  Packet *buildPacket();
  void close();
  bool isHTTP();
  void sendPacket(const Packet &packet);
  
  private:        /* variables */
  
  Gateway         gateway;
  GatewayVec      gateways;
  GatewayVecItr   itr;
  QTcpSocket     *socket;       /* TCP only */
  State           state;
};

}

}

#endif


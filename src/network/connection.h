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

#include <QHttp>
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

class Connection : public QObject
{
  Q_OBJECT

  public:         /* class specific */
	
  Connection();
  ~Connection();
  
  enum State {
    CONNECTED,
    CONNECTING,
    DISCONNECTING,
    DISCONNECTED
  };

  signals:
  
  void outgoingError(const QString&);
  void outgoingPacket(const QByteArray&);
  void outgoingState(Network::Connection::State);

  public:         /* methods */
  
  Packet *buildPacket();
  void close();
  State getState();
  bool isHTTP();
  void open(const Packet *login);
  void setGateway(const QString &connectionString);
  void sendPacket(const Packet *packet);

  private slots:
  
  void HTTP_connect();
  void HTTP_read(int id, bool error);
  void TCP_connect();
  void TCP_connected();
  void TCP_disconnect();
  void TCP_disconnected();
  void TCP_error(QAbstractSocket::SocketError socketError);
  void TCP_read();

  private:        /* variables */
  
  QByteArray      buffer;
  Gateway         gateway;
  QHttp           http;
  Packets::TCP    login;        /* TCP reconnect */
  QTcpSocket     *socket;       /* TCP only */
  State           state;
};

}

}

#endif


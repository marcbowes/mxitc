/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QUrl>

#include "connection.h"

namespace MXit
{

namespace Network
{

/****************************************************************************
        __                                _ ____    
   ____/ /__ ____ ___   ___ ___  ___ ____(_) _(_)___
  / __/ / _ `(_-<(_-<  (_-</ _ \/ -_) __/ / _/ / __/
  \__/_/\_,_/___/___/ /___/ .__/\__/\__/_/_//_/\__/ 
                         /_/                        
                         
****************************************************************************/


/****************************************************************************
**
** Author: Marc Bowes
**
** Connection constructor
**
****************************************************************************/
Connection::Connection()
  : state (DISCONNECTED)
{
  /* HTTP setup */
  connect(&http,    SIGNAL(requestFinished(int, bool)),
          this,    SLOT(HTTP_read(int, bool)));
  
  /* TCP setup */
  socket = new QTcpSocket();
  connect(socket,   SIGNAL(readyRead()),    this,   SLOT(TCP_read()));
  connect(socket,   SIGNAL(connected()),    this,   SLOT(TCP_connected()));
  connect(socket,   SIGNAL(disconnected()), this,   SLOT(TCP_disconnected()));
  connect(socket,   SIGNAL(error(QAbstractSocket::SocketError)),
          this,    SLOT(TCP_error(QAbstractSocket::SocketError)));
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Connection deconstructor
**
****************************************************************************/
Connection::~Connection()
{
  delete socket; /* will close if open */
}


/****************************************************************************
                __   ___                 __  __           __  
     ___  __ __/ /  / (_)___  __ _  ___ / /_/ /  ___  ___/ /__
    / _ \/ // / _ \/ / / __/ /  ' \/ -_) __/ _ \/ _ \/ _  (_-<
   / .__/\_,_/_.__/_/_/\__/ /_/_/_/\__/\__/_//_/\___/\_,_/___/
  /_/                                                         

****************************************************************************/


/****************************************************************************
**
** Author: Marc Bowes
**
** builds a packet of the appropriate sub-class
** this packet needs to be cleaned up at a later stage
**
****************************************************************************/
Packet* Connection::buildPacket() { 
  /* setup */
  Packet *packet = NULL;
  
  /* determine type of packet to be created */
  switch (gateway.type) {
    case Gateway::HTTP:
      packet = new Packets::HTTP();
      break;
    case Gateway::TCP:
      packet = new Packets::TCP();
      break;
    default:
      emit outgoingError("Invalid Gateway");
      break;
  }
  
  return packet;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** drops an active TCP connection
**
****************************************************************************/
void Connection::close()
{
  state = DISCONNECTING;
  buffer.clear();
  
  /* determine type of connection to open */
  switch (gateway.type) {
    case Gateway::HTTP:
      emit outgoingState(DISCONNECTED);
      break;
    case Gateway::TCP:
      TCP_disconnect();
      break;
    default:
      emit outgoingError("Invalid Gateway");
      return;
  }
}


/****************************************************************************
**
** Author: Marc Bowes
**
** returns the current state
**
****************************************************************************/
Connection::State Connection::getState()
{
  return state;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** returns true if current gateway is of type HTTP
**
****************************************************************************/
bool Connection::isHTTP()
{
  return gateway.type == Gateway::HTTP;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** calls the appropriate connect method (based on gateway)
**
****************************************************************************/
void Connection::open(const Packet *login)
{
  /* determine type of connection to open */
  switch (gateway.type) {
    case Gateway::HTTP:
      HTTP_connect();
      break;
    case Gateway::TCP:
      TCP_connect();
      break;
    default:
      emit outgoingError("Invalid Gateway");
      return;
  }
  state = CONNECTING;
  emit outgoingState(CONNECTING);
  sendPacket(login);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** sets the gateway to connect through
**
****************************************************************************/
void Connection::setGateway(const QString &connectionString)
{
  gateway.build(connectionString);
}


/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/
void Connection::sendPacket(const Packet *packet)
{
  if (state != CONNECTED) {
    if (gateway.type == Gateway::TCP) {
      if (state != CONNECTING)
        emit outgoingError("No connection has been opened");
      else
        socket->waitForConnected();
    }
    
    /* determine type of connection to open */
    switch (gateway.type) {
      case Gateway::HTTP:
        /* FIXME: anything in particular? */
        break;
      case Gateway::TCP:
        socket->waitForConnected();
        break;
      default:
        emit outgoingError("Invalid Gateway");
        delete packet;
        return;
    }
  }
  
  /* determine type of connection to open */
  switch (gateway.type) {
    case Gateway::HTTP:
      http.get(QString("%1?%2").arg(gateway.host).arg(QString(*packet)));
      break;
    case Gateway::TCP:
      socket->write(*packet);
      break;
    default:
      emit outgoingError("Invalid Gateway");
      delete packet;
      return;
  }
  
  delete packet;
}


/****************************************************************************
               _           __            __     __    
     ___  ____(_)  _____ _/ /____   ___ / /__  / /____
    / _ \/ __/ / |/ / _ `/ __/ -_) (_-</ / _ \/ __(_-<
   / .__/_/ /_/|___/\_,_/\__/\__/ /___/_/\___/\__/___/
  /_/                                                 

****************************************************************************/


/****************************************************************************
**
** Author: Marc Bowes
**
** HTTP connection to a gateway
**
****************************************************************************/
void Connection::HTTP_connect()
{
  /* check legitimacy of gateway */
  if (gateway.host.isEmpty() || gateway.port == 0 || gateway.type != Gateway::HTTP) {
    emit outgoingError("Invalid HTTP Gateway");
    return;
  }
  
  QUrl url(gateway.host);
  http.setHost(url.host(), gateway.port);
}


/****************************************************************************
**
** Author: Marc Bowes
** Author: Tim Sjoberg
**
** emits one HTTP packet at a time
**
****************************************************************************/
void Connection::HTTP_read(int id, bool error)
{ 
  buffer.append(http.readAll());
  if (!buffer.isEmpty()) {
    QList<QByteArray> packets = buffer.split('\2');
    int idx = 0, idx_27 = -1;   /* Packet 27 has data split by \2, */
    QByteArray packet27;        /* but it always the last packet */
    Q_FOREACH(const QByteArray &packet, packets) {
      idx++;
      if (packet.left(2) == "27")
        idx_27 = idx;
      if (idx_27 == -1) {
        emit outgoingPacket(packet);
      } else {
        packet27.append(packet);
        packet27.append('\2');
      }
    }
    if (!packet27.isEmpty()) {
      packet27.chop(1);
      emit outgoingPacket(packet27);
    }
  }
  buffer.clear();
}


/****************************************************************************
**
** Author: Marc Bowes
**
** TCP connection to a gateway
**
****************************************************************************/
void Connection::TCP_connect()
{
  /* check legitimacy of gateway */
  if (gateway.host.isEmpty() || gateway.port == 0 || gateway.type != Gateway::TCP) {
    emit outgoingError("Invalid TCP Gateway");
    return;
  }
  
  socket->connectToHost(gateway.host, gateway.port);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** SLOT to receive socket's connected SIGNAL
**
****************************************************************************/
void Connection::TCP_connected()
{
  state = CONNECTED;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** closes the socket
**
****************************************************************************/
void Connection::TCP_disconnect()
{
  socket->disconnectFromHost();
}


/****************************************************************************
**
** Author: Marc Bowes
**
** attempts to reconnect if connection was dropped
**
****************************************************************************/
void Connection::TCP_disconnected()
{
  state = DISCONNECTED;
  emit outgoingState(DISCONNECTED);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** takes socket error and emits a string error
** FIXME: hand error here not in client?
**
****************************************************************************/
void Connection::TCP_error(QAbstractSocket::SocketError socketError)
{
  emit outgoingError(socket->errorString());
}


/****************************************************************************
**
** Author: Marc Bowes
**
** emits one TCP packet at a time
**
****************************************************************************/
void Connection::TCP_read()
{ 
  forever { /* break statements for exit */
    buffer.append(socket->readAll());     /* buffer all input */
    
    if (buffer.isEmpty())
      break;
    
    /* need to find a packet starting with ln= */
    int idx_ln = buffer.indexOf("ln=");
    
    /* no ln= means our buffer is meaningless - this should not happen, but we
     * cannot recover from it */
    if (idx_ln < 0) {
      break;
    }
    
    /* x gives us the size of the packet, need to work out what x is */
    int idx_dl = buffer.indexOf('\0', idx_ln + 1);
    
    /* ensure \0 was found */
    if (idx_dl < 0) {
      /* need to wait on more data */
      break;
    }
    
    int ln = buffer.mid(idx_ln + 3, idx_dl - idx_ln - 3).toInt();
    int real_length = idx_dl + 1 + ln;
    
    /* now to check if all the data in the packet has been read */
    if (buffer.size() < real_length) {
      break;
    }
    
    QByteArray packet = buffer.mid(idx_ln, real_length);
    emit outgoingPacket(packet);
    buffer.remove(0, idx_ln + packet.size());
  }
}

}

}


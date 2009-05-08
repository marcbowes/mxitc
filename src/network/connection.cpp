/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "connection.h"

namespace MXit
{

namespace Network
{

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
  /* TCP setup */
  socket = new QTcpSocket();
  connect(socket,   SIGNAL(readyRead()),    this,   SLOT(incomingPacket()));
  connect(socket,   SIGNAL(connected()),    this,   SLOT(TCP_connected()));
  connect(socket,   SIGNAL(disconnected()), this,   SLOT(TCP_disconnected()));
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


/****************************************************************************
**
** Author: Marc Bowes
**
** forwards read data to the client
**
****************************************************************************/
void Connection::incomingPacket()
{
  /* FIXME: TCP/HTTP */
  TCP_read();
}


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
  
  /* determine type of connection to open */
  switch (gateway.type) {
    case Gateway::HTTP:
      /* TODO: HTTP */
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
      /* TODO: HTTP */
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
****************************************************************************/
void Connection::sendPacket(const Packet *packet)
{
  if (state != CONNECTED) {
    if (state != CONNECTING) {
      emit outgoingError("No connection has been opened");
    }
    
    /* determine type of connection to open */
    switch (gateway.type) {
      case Gateway::HTTP:
        /* TODO: HTTP */
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
      /* TODO: HTTP */
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

}

}


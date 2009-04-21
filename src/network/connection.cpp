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
{
  socket = new QTcpSocket();
  
  /* TCP reconnect when disconnected */
  connect(socket, SIGNAL(disconnected()), this, SLOT(TCP_disconnected()));
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
** attempts to reconnect
** TODO: cycle gateways
**
****************************************************************************/
void Connection::TCP_disconnected()
{
  TCP_connect();
}


/****************************************************************************
**
** Author: Marc Bowes
**
** adds a gateway to the connection
** (not thread-safe)
**
****************************************************************************/
void Connection::addGateway(const QString &connectionString)
{
  Gateway gateway (connectionString);
  gateways.append(gateway);
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
  /* FIXME HACK */
  gateway = gateways.first();
  
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
  }
  
  return packet;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** thread-safe method to enqueue a message
**
****************************************************************************/
void Connection::enqueue(const Packet &packet)
{
  queueMutex.lock();        /* wait for a lock on the queue */
  queue.append(packet);     /* enqueue the packet */
  queueWait.wakeOne();      /* wake up the event loop if its sleeping */
  queueMutex.unlock();      /* release the mutex */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** called by start()
** event loop for dealing with incoming/outgoing data
**
****************************************************************************/
void Connection::run()
{
  socket->moveToThread(this);
  TCP_connect();
  
  while (true) {  /* FIXME: check state */
    queueMutex.lock();        /* wait for a lock on the queue */
    
    /* check for messages to send */
    if (queue.isEmpty()) {
      queueWait.wait(&queueMutex);
    }
    
    /* FIXME: this is a hack */
    socket->waitForConnected();
    
    /* write all messages into a stream */
    QTextStream out(socket);
    ByteArrayVecItr itr(queue);
    while (itr.hasNext()) {   /* iterate over queue */
      out << itr.next();      /* write to stream */
    }
    
    /* need to flush the stream, otherwise data will go out of scope before
     * it is actually sent */
    out.flush();
    while (socket->bytesToWrite() && socket->waitForBytesWritten());
    
    queueMutex.unlock();      /* release the mutex */
  }
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
  socket->connectToHost(gateway.host, gateway.port);
}

}

}


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
  /* instantiation done by thread start to ensure objects belong to correct
    thread */
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
** emits read data
** FIXME: this seems dirtier than need be
**
****************************************************************************/
void Connection::TCP_read()
{
  /* read all available data */
  QTextStream in (socket);
  buffer.append(in.readAll());
  
  qDebug() << "Buffer : " << buffer.replace(QChar('\0'), QString("\\0")).replace(QChar('\1'), QString("\\1")).replace(QChar('\2'), QString("\\2"));
  /* split by packets (record terminator is \2) */
  QStringList packets = buffer.split("\2");
  
  /* clear the buffer */
  buffer.clear();
  
  /* for each record, need to emit a signal */
  Q_FOREACH(const QString &packet, packets) {
    /* ensure the packet is terminated */
    if (packet.indexOf("\2") != -1) {
      
      QByteArray _packet; _packet.append(packet);
      
      emit outgoingPacket(_packet);
    } else {
      /* rebuffer it */
      buffer.append(packet);
    }
  }
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
  /* Qt requires objects to be in the same thread - construction here saves
   * a call to moveToThread */
  socket = new QTcpSocket();
  TCP_connect(); /* FIXME: move to abstraction layer */
    
  while (true) {  /* FIXME: check state */
    queueMutex.lock();        /* wait for a lock on the queue */
    
    /* check for messages to send */
    if (queue.isEmpty()) {
      queueWait.wait(&queueMutex);
    }
    
    /* FIXME: this is a hack */
    socket->waitForConnected();
    
    /* write first message in queue */
    socket->write(queue.first());      /* write to stream */
    
    /* need to flush the stream, otherwise data will go out of scope before
     * it is actually sent */
    while (socket->bytesToWrite() && socket->waitForBytesWritten());
    
    /* dequeue message */
    queue.erase(queue.begin());
    
    queueMutex.unlock();      /* release the mutex */
    
    /* wait for reply */
    socket->waitForReadyRead();
    TCP_read();
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


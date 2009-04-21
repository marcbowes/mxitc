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
  delete socket;
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Returns a new (dynamically allocated) Packet of the appopriate subclass 
** type, this will (should) be cleaned up from memory once sent
**
****************************************************************************/

Packet* Connection::getNewPacket() {
  /* only tcp for now. TODO add http */
  
  return new TCPPacket();
  
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
  queueMutex.lock();        /* wait for a lock on the queue */
  
  /* check for messages to send */
  if (queue.isEmpty()) {
    queueWait.wait(&queueMutex);
  }
  
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

}


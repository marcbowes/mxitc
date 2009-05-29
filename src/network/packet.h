/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Network::Packet blueprints functionality provided by various packet
** implementations.
**
****************************************************************************/

#ifndef __MXIT_NETWORK_PACKET_H__
#define __MXIT_NETWORK_PACKET_H__

#include <QList>
#include <QListIterator>

#include "common/types.h"

namespace MXit
{

namespace Network
{

typedef QList<QByteArray> ByteList;

class Packet
{
  public:           /* class specific */
  
  Packet(const QString &cellphone = "", const QString &commandNumber = "");
  virtual ~Packet() = 0;

  public:           /* methods */
  
  Packet& operator<<(const QByteArray &message);
  virtual operator QByteArray() const = 0;
  virtual void setCellphone(const QString &cellphone);
  virtual void setCommand(const QString &command);
  virtual void setPostMs(const QByteArray &extra);

  protected:         /* methods */
  
  QByteArray getData() const;
  
  protected:       /* variables */
  
  QString     cellphone;
  QString     command;
  ByteList    data;
  QByteArray  extra;
};

}

}

#endif


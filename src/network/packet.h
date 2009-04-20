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

#include "common/types.h"

namespace MXit
{

namespace Network
{

class Packet
{
  public:           /* methods */
  
  QString getData() const;
  void operator<<(const QString &message);
  virtual operator QByteArray() const = 0;
  void setCellphone(const QString &cellphone);
  void setCommand(const QString &command);
  
  protected:       /* variables */
  
  QString   cellphone;
  QString   command;
  StringVec data;
};

}

}

#endif


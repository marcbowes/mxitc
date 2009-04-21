/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Protocol::Gateway encapsulates a MXit gateway (TCP/HTTP)
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_GATEWAY_H__
#define __MXIT_PROTOCOL_GATEWAY_H__

#include <QString>

namespace MXit
{

namespace Network
{

class Gateway
{
  public:           /* class specific */
  
  Gateway();
  Gateway(const QString &gateway);
  ~Gateway();
  
  enum Type {
    HTTP,
    TCP
  };

  public:           /* methods */
  
  void build(const QString &gateway);
  
  public:           /* variables */
  
  /* TCP */
  QString hostName;
  quint16 port;
  
  /* HTML */
  QString URL;  
  
  Type    type;
};

}

}

#endif


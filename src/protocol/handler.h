/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Network::Handler is a blueprint for building command handlers
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLER_H__
#define __MXIT_PROTOCOL_HANDLER_H__

#include <QObject>

namespace MXit
{

namespace Protocol
{

class Handler : public QObject
{
  Q_OBJECT
  
  signals:
  
  void handled(); /* used to deleteLater() */
  
  public:         /* methods */
  
  virtual void handle() = 0;
};

}

}

#endif


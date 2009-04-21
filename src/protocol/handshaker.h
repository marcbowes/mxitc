/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
** Author: Richard Baxter, 2009
** Author: Timothy Sjoberg, 2009
**
** MXit::Protocol::Handshaker provides functionality to get a PID via a 
** CAPTCHA negotiation
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDSHAKER_H__
#define __MXIT_PROTOCOL_HANDSHAKER_H__

#include <QHttp>

#include "common/types.h"

namespace MXit
{

namespace Protocol
{

class Handshaker : public QObject
{
  Q_OBJECT

  public:         /* class specific */

  Handshaker();
  ~Handshaker();
  
  enum Status {
    IDLE,
    INITIALIZING,
    CHALLENGING
  };

  signals:
        
  void outgoingVariables(const VariableHash &);
        
  private slots:
  
  void requestComplete(int id, bool error);

  public:         /* methods */
  
  void challenge(const QString &cellphone, const QString &captcha,
    const QString &_url, const QString &_sessionid);
  void initialize();
  
  private:        /* methods */
  
  void challengeReceived(const QByteArray &response);
  VariableHash hashResponse(const QByteArray &response, const StringVec &variables,
    const QString &delimiter = ";");
  int responseError(const QByteArray &response,
    const QString &delimiter = ";");
  void setupReceived(const QByteArray &response);

  private:        /* variables */
        
  int             currentRequest;
  QHttp          *http;
  Status          state;
};

}

}

#endif


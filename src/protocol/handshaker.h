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

#include "common/hash_variables.h"

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

  public:         /* methods */
  
  void challenge(const QString &captcha, const VariableHash &settings, bool signup);
  void initialize();
  void setProxy(const QString &host, quint16 port,
    const QString &username, const QString &password);
  
  private slots:
  
  void requestComplete(int id, bool error);
  
  private:        /* methods */
  
  void challengeReceived(const QByteArray &response);
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


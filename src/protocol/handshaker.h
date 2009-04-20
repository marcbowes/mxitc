/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Richard Baxter, 2009
** Author: Timothy Sjoberg, 2009
** Author: Marc Bowes, 2009
**
** MXit::Protocol::Handshaker provides functionality to get a PID via a 
** CAPTCHA negotiation
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDSHAKER_H__
#define __MXIT_PROTOCOL_HANDSHAKER_H__

#include <QHttp>
#include <QVector>
#include <QVectorIterator>
#include <QHash>

namespace MXit
{

namespace Protocol
{

typedef QVector<QString> StringVec;
typedef QVectorIterator<QString> StringVecItr;
typedef QHash<QString, QString> StringHash;

class Handshaker : public QObject
{
  Q_OBJECT

  public:         /* class specific */

  Handshaker();
  ~Handshaker();
  
  enum Status {
    IDLE,
    INITIALIZING,
    REQUESTING_PID
  };

  signals:
        
  void outgoingVariables(const StringHash &);
        
  private slots:
  
  void requestComplete(int requestId, bool error);

  public:         /* methods */
  
  void challenge(const QString &cellphone, const QString &captcha);
  void initialize();
  
  private:        /* methods */
  
  void captchaReceived(const QByteArray &response);
  StringHash hashResponse(const QByteArray &response, const StringVec &variables, const QString &delimiter = ";");
  void PIDReceived(const QByteArray &response);

  private:        /* variables */
        
  int             currentRequest;
  QHttp          *http;
  QString         PIDURL;
  QString         SESSIONID;
  Status          state;
};

}

}

#endif


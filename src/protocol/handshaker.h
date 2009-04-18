/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Richard Baxter, 2009
** Author: Timothy Sjoberg, 2009
** Author: Marc Bowes, 2009
**
** Handshaker provides functionality to get a PID via a CAPTCHA negotiation
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDSHAKER_H__
#define __MXIT_PROTOCOL_HANDSHAKER_H__

#include <QHttp>

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
    NONE,
    CHALLENGE_INITIAL,
    CHALLENGE_RESPONSE
  };

  signals:
        
  void outgoingCaptcha(const QByteArray &);
        
  private slots:
  
  void requestComplete(int requestId, bool error);

  public:         /* methods */
  
  void initialize();
  void requestPID(const QString &cellphone, const QString &captcha);
  
  private:        /* methods */
  
  void captchaReceived(const QByteArray &response);
  QByteArray extractDataFromResponse(const QByteArray &response, unsigned int index, const QString &delimiter = ";");

  private:        /* variables */
        
  QByteArray      challengeResponseURL;
  QHttp          *http;
  int             httpGetId;
  QByteArray      sessionID;
  Status          state;
};

}

}

#endif


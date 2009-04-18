/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** This is an extension of the generated dialog, made with Designer
**
****************************************************************************/

#ifndef __HTTPCOMM_H__
#define __HTTPCOMM_H__

#include <QHttp>
#include <QWaitCondition>
#include <QMutex>

namespace MXit
{

class HttpComm : public QObject
{
  Q_OBJECT

  public:         /* class specific */

  HttpComm(QObject *parent = 0);

  signals:
        
  void captchaReceived(const QByteArray &);
        
  private slots:
        
  void httpRequestStarted(int requestId);
  void httpRequestFinished(int requestId, bool error);

  public:         /* methods */
  
  void sendInitialChallenge();
  void sendChallengeResponse();
  
  private:        /* methods */
  
  QByteArray extractDataFromResponce(int data_num);

  private:        /* variables */
        
  QHttp          *http;

  enum currentHttpGet_t {NONE, CHALLENGE_INITIAL, CHALLENGE_REPONSE};
  currentHttpGet_t currentHttpGet;

  int             httpGetId;
  QByteArray      responseByteArray;
  
  
  QString         challengeResponseURL;
};

}

#endif


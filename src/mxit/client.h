/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** This is an extension of the generated dialog, made with Designer
**
****************************************************************************/

#ifndef __CLIENT_H__
#define __CLIENT_H__

#include<QObject>

namespace MXit
{

class Client : public QObject
{
  Q_OBJECT

  public:         /* class specific */
	
  Client(QObject *parent = 0);

<<<<<<< HEAD:src/mxit/client.h
	

  public:         /* methods */
    
  private:        /* methods */

  private:        /* variables */

=======
	signals:
	
  void captchaReceived(const QByteArray &);
	
	private slots:
	
  void httpRequestStarted(int requestId);
  void httpRequestFinished(int requestId, bool error);

  public:         /* methods */
  
  void getLoginCaptcha();
  void sendChallengeResponse(QString captchaResponse, QString login);
    
  private:        /* methods */
  
  QByteArray extractDataFromResponce(int data_num);

	private:        /* variables */
	
  QMutex         *captchaMutex;
  QWaitCondition *captchaWaitCond;
  QHttp          *http;
  int             httpGetId;
  QByteArray      responseByteArray;
>>>>>>> 68e02e608c65acfd1b1424b7277151a6894e7d63:src/mxit/client.h
};

}

#endif


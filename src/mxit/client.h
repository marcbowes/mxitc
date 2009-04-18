/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** This is an extension of the generated dialog, made with Designer
**
****************************************************************************/

#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <QHttp>
#include <QWaitCondition>
#include <QMutex>

namespace MXit
{

class Client : public QObject
{
  Q_OBJECT

	public:         /* class specific */
	
	Client(QObject *parent = 0);

	signals:
	
  void captchaReceived(const QByteArray &);
	
	private slots:
	
  void httpRequestStarted(int requestId);
  void httpRequestFinished(int requestId, bool error);

  public:         /* methods */
  
  void getLoginCaptcha();
  void sendCaptchaResponse(const QString &text);
  
  private:        /* methods */
  
  QByteArray extractDataFromResponce(int data_num);

	private:        /* variables */
	
  QMutex         *captchaMutex;
  QWaitCondition *captchaWaitCond;
  QHttp          *http;
  int             httpGetId;
  QByteArray      responseByteArray;
};

}

#endif


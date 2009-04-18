#ifndef MXIT_H
#define MXIT_H

#include <QObject>
//#include <QTcpSocket>
#include <QHttp>
#include <QFile>
#include <QWaitCondition>
#include <QMutex>

namespace MXit
{

class Client : public QObject
{
Q_OBJECT

	public:
		Client(QObject *parent = 0);

	signals:
	  void captchaReceived(const QByteArray &);
	
	private slots:
	  void httpRequestStarted(int requestId);
	  void httpRequestFinished(int requestId, bool error);
		
	private:
	  QWaitCondition *captchaWaitCond;
	  QMutex *captchaMutex;
	  
	  QHttp *http;
	  int httpGetId;
//		QTcpSocket *socket;
//		QString host;
//		quint16 port;

  public:
  
  void getLoginCaptcha();
  void sendCaptchaResponse(const QString &text);
};

}

#endif

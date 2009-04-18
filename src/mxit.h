#ifndef MXIT_H
#define MXIT_H

#include <QObject>
//#include <QTcpSocket>
#include <QString>

class Mxit : public QObject
{
Q_OBJECT

	public:
		Mxit(QObject *parent = 0);

//	private slots:
		
//	private:
//		QTcpSocket *socket;
//		QString host;
//		quint16 port;

  public:
  
  QByteArray getLoginCaptcha();
  void sendCaptchaResponse(const QString &text);
};

#endif

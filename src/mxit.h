#ifndef MXIT_H
#define MXIT_H

#include <QObject>
//#include <QTcpSocket>
#include <QHttp>
#include <QFile>

class Mxit : public QObject
{
Q_OBJECT

	public:
		Mxit(QObject *parent = 0);

	private slots:
	  void httpRequestFinished(int requestId, bool error);
		
	private:
	  QHttp *http;
	  int httpGetId;
//		QTcpSocket *socket;
//		QString host;
//		quint16 port;
};

#endif

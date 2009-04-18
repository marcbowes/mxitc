#include "mxit.h"
#include <QString>
#include <QUrl>
#include <QDateTime>
#include <QDebug>

Mxit::Mxit(QObject *parent) : QObject (parent)
{
  QString timestamp = QString("%1").arg(QDateTime::currentDateTime().toTime_t());
  //QUrl url("http://www.mxit.com/res/");//?type=challenge&getcountries=true&getlanguage=true&getimage=true&ts=" + timestamp);
  QUrl url("http://war3.co.za/forum/viewforum.php");
  http = new QHttp(this);
    
  connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
  
  QByteArray query;
  query += url.path().toLatin1();
  //query += "?type=challenge&getcountries=true&getlanguage=true&getimage=true&ts=";
  //query += timestamp;
  query += "?f=26&sid=194feef8a58e637d463271796dd23d22";
  
  qDebug() << query << "\n";
  
  http->setHost(url.host(), url.port() == -1 ? 0 : url.port());
  httpGetId = http->get(query);
}

void Mxit::httpRequestFinished(int requestId, bool error)
{
  QByteArray response = http->readAll();
  QString temp(response);
  if (!error)
    qDebug() << "success\n";
  else
    qDebug() << "err0r\n";
  qDebug() << temp << "\n";
}

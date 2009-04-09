#include "mxit.h"
#include <QString>
#include <QUrl>
#include <QHttp>
#include <QFile>
#include <QDateTime>
#include <iostream>
#include <string>

Mxit::Mxit(QObject *parent) : QObject (parent)
{
  QString timestamp = QString("%1").arg(QDateTime::currentDateTime().toTime_t());
  QUrl url("http://www.mxit.com/res/?type=challenge&getcountries=true&getlanguage=true&getimage=true&ts=" + timestamp);
  //QHttp http(this);
  //QFile file("testfile");
  
  std::cout << url.toString().toStdString() << std::endl;
}

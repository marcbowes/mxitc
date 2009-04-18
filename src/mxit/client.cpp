#include <QString>
#include <QUrl>
#include <QDateTime>
#include <QDebug>

#include "client.h"

namespace MXit
{

#define DEBUG(x) qDebug() << #x << ":\t" << x;

Client::Client(QObject *parent) : QObject (parent)
{
  http = new QHttp(this);
  captchaWaitCond = new QWaitCondition();
  captchaMutex = new QMutex();
    
  connect(http, SIGNAL(requestStarted(int)), this, SLOT(httpRequestStarted(int)));
  connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
}


void Client::httpRequestStarted(int requestId)
{
  qDebug() << "Request Started";
}



void Client::httpRequestFinished(int requestId, bool error)
{
  qDebug() << "Request Finished";

  if (requestId == httpGetId) {

//     QByteArray response = http->readAll();

    DEBUG(httpGetId);
    DEBUG(requestId);
    DEBUG(http->lastResponse().statusCode());


    if (!error) {
      qDebug() << "success";
      responseByteArray = http->readAll();
      
    }
    else
      qDebug() << "err0r";

    DEBUG(http->errorString());
  }
}

QByteArray Client::getLoginCaptcha()
{
  QString timestamp = QString("%1").arg(QDateTime::currentDateTime().toTime_t());
  QUrl url("http://www.mxit.com");//?type=challenge&getcountries=true&getlanguage=true&getimage=true&ts=" + timestamp);
  
  QByteArray query;
  query += url.path();
  //query += "?type=challenge&getcountries=true&getlanguage=true&getimage=true&ts=";
  //query += timestamp;
  query += "/res/?type=challenge&getcountries=true&getlanguage=true&getimage=true&ts=" + timestamp;
  
  DEBUG(url.host());
  DEBUG(query);
  
  http->setHost(url.host(), 80 );

//   DEBUG(http->readAll());

  httpGetId = http->get(query);
  

  return QByteArray::fromBase64(
  
  "iVBORw0KGgoAAAANSUhEUgAAAFAAAAAUCAYAAAAa2LrXAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAAA+hJREFUWEftmD1II1EQxy82KihaRFGxiHCCZapgYWFhIVbaiYUIQtAuoIWNWAQJ2NgIlhYWlmJlaaeNYCnBgJ1eZ2GRcs7fcCPry9vdt17CfeDAY7PvY97Mfz43uf7+/ofX19fv374oMwJjY2M/OCT/M93d3cnV1ZUcHh7K3t6eLC8vy+zsrIyMjAi6T01N6fva2pqun5yc6P7Hx8dUWDj/zwN4c3OjCtdqNQVgfn5eARkcHFSAisWivq+srEi5XJazszPd//z8nApQ2oaOA/j09CTb29uytLSkz6zUbDZVWQbgMACD8Ut4mZ6e1vednR1dv7y81P0vLy963e3trUxOTn4Yp6eniaIg9/7+vszMzKjcvEN4c6VSUW+FYgHkUPRSmGUlLl1fX5e3/Bp7FCVR9vz8XJUHBMAAFITL5XLS29urg1Db2tp6BzRUHvRADiN+M2eguHyYtzPovbCwIAMDA5LP59WbSQUW3l4A8RYYHB0dqfVgwsaoECHCY4RGo6EKEzYAtLGxoQAhCDwJM94XFxd1nTBk/8HBwfudKMQcMuERWYhzvjPwQjcfIffo6Kh6mck3PDwsm5ubLdtbAKzX6yq46+IGIushZCHGxRMTE+pRAHR8fKxgEApJhNKu4siE4qEyxPHnfnSM8iFVYGQKTHd3t8zNzWkxsTSALD4HagEQ5nigS1iLzXFW8wnLhZb34BuaBsyIREA7CX6WmngCDiDh/T09PQoeIAJmlDiH4dDBpeAiYhfH5Q2foq4H+QzjO4enmbH4bYbwKZAFYPiUSiUhKshnhCdhSv6NI4BLSl9BAFoVy1pF3f2hAOKplsStCvL0pZYQAEn4JH7yLr3f6uqqjI+PB+d0IoL7ffKnAmjgZU3eKMbFBiJhkNY6GBiWb92QT6ueUTDv7+8151K5C4WCth7RvGsRFWIA9vjyJvOJAFrS/gx4JhghD3hZws88MC4Px+VG69EADOAAcHd3V73ZJTOGD0D4X1xcfFiyGuAWsFgAP9u6hFo0aZ9Z2y1YpkTUM9lLa0Rouj1a1HOioFtUxRU1H7jM+RzJC6CBlzXntQM8eFgj695vwFar1Q89Gq1R0mdZNJIAwW2s4wqg9Z12xteBeNsYJkmYHHBHu0BK42NewP20FbQbQ0ND+mVC2xHt0dJ4mVHwQpwjtI+01ispd7cAiOD2jel7ZukDQxSL20OPhgf09fVJV1eXvP3lpmF6fX39O2zbftb7JeLzPJtruwQRhoQh4Wj/pNCjEa54TWgF76R8Pt6pbUynBXJ7NApClordafnS+P8RANN6tDSh/6Z1BfDtO7CZlPe+1vRfZ+/gc/AnfV2hi6D9M9AAAAAASUVORK5CYII="
  
  );
}

void Client::sendCaptchaResponse(const QString &text)
{
  // stub
}

}

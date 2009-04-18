/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** This is an extension of the generated dialog, made with Designer
**
****************************************************************************/

#include <QUrl>
#include <QDateTime>
#include <QDebug>

#include "client.h"

namespace MXit
{

#define DEBUG(x) qDebug() << #x << ":\t" << x;

HttpComm::HttpComm(QObject *parent) : QObject (parent)
{
  http = new QHttp(this);
  currentHttpGet = NONE;
    
  connect(http, SIGNAL(requestStarted(int)), this, SLOT(httpRequestStarted(int)));
  connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
}

///=============================================================================

void HttpComm::httpRequestStarted(int requestId)
{
  qDebug() << "Request Started";
}

///- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void HttpComm::httpRequestFinished(int requestId, bool error)
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
      if (currentHttpGet == CHALLENGE_INITIAL) {
        //TODO add other functionality here
        challengeResponseURL = extractDataFromResponce(1);
        DEBUG(challengeResponseURL);
        emit captchaReceived(QByteArray::fromBase64(extractDataFromResponce(3)));
      }
    } else {
      qDebug() << "err0r";
      DEBUG(http->errorString());
    }
    
  }

}
///=============================================================================

QByteArray HttpComm::extractDataFromResponce(int data_num) {
  
  QString delim = ";";
  int start = 0;
  int end = responseByteArray.indexOf(delim);
  for (int i = 0 ; i < data_num ; i++) {
    start = end+1;
    end = responseByteArray.indexOf(delim, start);
  }
  return responseByteArray.mid(start, end-start);

}

///=============================================================================

void HttpComm::sendInitialChallenge()
{
  QString timestamp = QString("%1").arg(QDateTime::currentDateTime().toTime_t());
  QUrl url("http://www.mxit.com/res/");
  
  QByteArray query;
  query += url.path();
  query += "?type=challenge&getcountries=true&getlanguage=true&getimage=true&ts=" + timestamp;
  
  DEBUG(url.host());
  DEBUG(query);
  
  http->setHost( url.host(), 80 );

  currentHttpGet = CHALLENGE_INITIAL;

  httpGetId = http->get(query);
}

///- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void HttpComm::sendChallengeResponse()
{
  // TODO ... like, need to rewrite EVERYTHING! for this function
  QString timestamp = QString("%1").arg(QDateTime::currentDateTime().toTime_t());
  QUrl url(challengeResponseURL);//TODO use address got in initial challenge
  
  QByteArray query;
  query += url.path();

  query += "?type=challenge&getcountries=true&getlanguage=true&getimage=true&ts=" + timestamp; // TODO change to proper data
  
  DEBUG(url.host());
  DEBUG(query);
  
  http->setHost( url.host(), 80 );

  currentHttpGet = CHALLENGE_RESPONSE;

  httpGetId = http->get(query);
}

///=============================================================================
}
/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QUrl>
#include <QDateTime>
#include <QDebug>

#include "handshaker.h"
#include "error_codes.h"

#define DEBUG(x) qDebug() << #x << ":\t" << x;

namespace MXit
{

namespace Protocol
{

/****************************************************************************
**
** Author: Richard Baxter
**
** Handshaker constructor
**
****************************************************************************/
Handshaker::Handshaker()
  : state (NONE)
{
  http = new QHttp(this);
  
  /* when an HTTP request completes */
  connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(requestComplete(int, bool)));
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Handshaker destructor
**
****************************************************************************/
Handshaker::~Handshaker()
{
  delete http;
}


/****************************************************************************
**
** Author: Richard Baxter
** Author: Marc Bowes
**
** this SLOT is triggered when an HTTP request completes
**
****************************************************************************/
void Handshaker::requestComplete(int requestId, bool error)
{
#ifdef HANDSHAKER_DEBUG
  qDebug() << "Request Finished";
  DEBUG(httpGetId);
  DEBUG(requestId);
  DEBUG(http->lastResponse().statusCode());
#endif
  
  /* check for error */
  if (error) {
    qDebug() << "There was an error in the HTTP request";
    DEBUG(http->errorString());
    
    return;
  }
  
  /* ensure the request that completed is the one we expected */
  if (requestId != httpGetId) {
    qDebug() << "unexpected request id";
    
    return;
  }
  
  QByteArray response = http->readAll();
#ifdef HANDSHAKER_DEBUG
  DEBUG(response);
#endif
  
  switch (state) {
    case CHALLENGE_INITIAL:
      captchaReceived(response);
      break;
    default:
      // TODO: CHALLENGE_RESPONSE
      break;
  }
}


/****************************************************************************
**
** Author: Richard Baxter
** Author: Marc Bowes
**
** utility method to find data at an index defined by a ; delimiter
**
****************************************************************************/
void Handshaker::initialize()
{
  QString timestamp = QString("%1").arg(QDateTime::currentDateTime().toTime_t());
  QUrl url("http://www.mxit.com/res/");
  http->setHost(url.host(), 80);
  
  QByteArray query;
  query += url.path();
  query += "?type=challenge&getcountries=true&getlanguage=true&getimage=true&ts=" + timestamp;

#ifdef HANDSHAKER_DEBUG
  DEBUG(url.host());
  DEBUG(query);
#endif
  
  state = CHALLENGE_INITIAL;
  httpGetId = http->get(query);
}


/****************************************************************************
**
** Author: Richard Baxter
** Author: Marc Bowes
**
** utility method to find data at an index defined by a ; delimiter
**
****************************************************************************/
void Handshaker::requestPID(const QString &cellphone, const QString &captcha)
{
  QUrl url(challengeResponseURL);
  QString sessionID_STR(sessionID);
  
  // FIXME: **way** more environment variables needed here
  QString query =
    QString("%1?type=getpid&sessionid=%2&ver=5.8.2&login=%3&cat=Y&chalresp=%4&cc=ZA&loc=en&brand=LPM&model=StrioClient&path=1")
    .arg(url.path())
    .arg(sessionID_STR)
    .arg(cellphone)
    .arg(captcha)
  ;
  
#ifdef HANDSHAKER_DEBUG
  DEBUG(url.host());
  DEBUG(query);
#endif
  
  http->setHost(url.host(), 80);

  state = CHALLENGE_RESPONSE;
  httpGetId = http->get(query);
}


/****************************************************************************
**
** Author: Richard Baxter
** Author: Marc Bowes
**
** this method is called by the requestComplete SLOT and is specific for when
** the request was for a CAPTCHA
**
** emits outgoingCaptcha if the CAPTCHA was successfully received
**
****************************************************************************/
void Handshaker::captchaReceived(const QByteArray &response)
{
  if (extractDataFromResponse(response, 0) != MXit::Protocol::ErrorCodes::NoError) {
    // FIXME: how to handle this?
  }
  
  challengeResponseURL = extractDataFromResponse(response, 1);
  sessionID = extractDataFromResponse(response, 2);
  
#ifdef HANDSHAKER_DEBUG
  DEBUG(challengeResponseURL);
  DEBUG(sessionID);
#endif
  
  emit outgoingCaptcha(QByteArray::fromBase64(extractDataFromResponse(response, 3)));
}


/****************************************************************************
**
** Author: Richard Baxter
** Author: Marc Bowes
**
** utility method to find data at an index defined by a ; delimiter
**
****************************************************************************/
QByteArray Handshaker::extractDataFromResponse(const QByteArray &response, unsigned int index, const QString &delimiter)
{
  unsigned int start = 0;
  unsigned int end = response.indexOf(delimiter);
  for (unsigned int i = 0 ; i < index ; i++) {
    start = end + 1;
    end = response.indexOf(delimiter, start);
  }
  
  return response.mid(start, end - start);
}

}

}


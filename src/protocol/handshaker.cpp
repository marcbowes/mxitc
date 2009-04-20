/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QUrl>
#include <QDateTime>
#include <QDebug>

#include "error_codes.h"

#include "handshaker.h"

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
  : state (IDLE)
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
void Handshaker::requestComplete(int id, bool error)
{
#ifdef HANDSHAKER_DEBUG
  qDebug() << "Request Finished";
  DEBUG(currentRequest);
  DEBUG(id);
  DEBUG(http->lastResponse().statusCode());
#endif
  
  /* check for error */
  if (error) {
    qDebug() << "There was an error in the HTTP request";
    DEBUG(http->errorString());
    
    return;
  }
  
  /* ensure the request that completed is the one we expected */
  if (id != currentRequest) {
  #ifdef HANDSHAKER_DEBUG
    qDebug() << "ignoring unexpected request id";
  #endif
    
    return;
  }
  
  QByteArray response = http->readAll();
#ifdef HANDSHAKER_DEBUG
  DEBUG(response);
#endif
  
  switch (state) {
    case INITIALIZING:
      challengeReceived(response);
      break;
    case CHALLENGING:
      setupReceived(response);
    default:
      // TODO: what here?
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
  
  state = INITIALIZING;
  currentRequest = http->get(query);
}


/****************************************************************************
**
** Author: Richard Baxter
** Author: Marc Bowes
**
** utility method to find data at an index defined by a ; delimiter
**
****************************************************************************/
void Handshaker::challenge(const QString &cellphone, const QString &captcha)
{
  QUrl url(PIDURL);
  
  // FIXME: **way** more environment variables needed here
  QString query =
    QString("%1?type=getpid&sessionid=%2&ver=5.8.2&login=%3&cat=Y&chalresp=%4&cc=ZA&loc=en&brand=LPM&model=StrioClient&path=1")
    .arg(url.path())
    .arg(SESSIONID)
    .arg(cellphone)
    .arg(captcha)
  ;
  
#ifdef HANDSHAKER_DEBUG
  DEBUG(url.host());
  DEBUG(query);
#endif
  
  http->setHost(url.host(), 80);

  state = CHALLENGING;
  currentRequest = http->get(query);
}


/****************************************************************************
**
** Author: Richard Baxter
** Author: Marc Bowes
**
** this method is called by the requestComplete SLOT and is specific for when
** the request was for a CAPTCHA
**
****************************************************************************/
void Handshaker::challengeReceived(const QByteArray &response)
{
  /* variable declarations for this response type */
  StringVec variables;
  variables.append("err");                  /* 0 = success, else failed */
  variables.append("url");                  /* URL that should be used for the Get PID request */
  variables.append("sessionid");            /* unique identifier to identify the session for code image answer */
  variables.append("captcha");              /* base64 encoded image data */
  variables.append("countries");            /* list of available countries (countrycode|countryname)
                                             * the list of country names should be presented to the user in order to
                                             * find the country code that should be used later on */
  variables.append("languages");            /* list of supported languages (locale|languagename)
                                             * the list of language names should be presented to the user and the
                                             * corresponding locale saved by the client to be used later on */
  variables.append("defaultCountryName");   /* country name of the country detenced from the requestors IP */
  variables.append("defaultCountryCode");   /* country code associated with the defaultCountryName */
  variables.append("regions");              /* a '|' seperated list of regions if requested */
  variables.append("defaultDialingCode");   /* dialing code associated with the defaultCountryName */
  variables.append("defaultRegion");        /* a region of the detected IP */
  variables.append("defaultNPF");           /* the national dialing prefix for the defaultCountryName, e.g. 0 */
  variables.append("defaultIPF");           /* the international dialing prefix for the defaultCountryName, e.g. 00 */
  variables.append("cities");               /* NOT IMPLEMENTED YET */
  variables.append("defaultCity");          /* the city of the detected IP */
  
  /* now to assign variable values from the response */
  VariableHash params = hashResponse(response, variables);
  
  if (params["err"] != MXit::Protocol::ErrorCodes::NoError) {
    // FIXME: how to handle this?
    
    return;
  }
  
  PIDURL    = params["url"];
  SESSIONID = params["sessionid"];
  
#ifdef HANDSHAKER_DEBUG
  DEBUG(PIDURL);
  DEBUG(SESSIONID);
#endif
  
  emit outgoingVariables(params);
}


/****************************************************************************
**
** Author: Marc Bowes
** Author: Richard Baxter
**
** returns a VariableHash
**
****************************************************************************/
VariableHash Handshaker::hashResponse(const QByteArray &response, const StringVec &variables,
  const QString &delimiter)
{
  /* setup */
  VariableHash params;                    /* to be returned */
  StringVecItr itr(variables);            /* for iteration */
  unsigned int start  = 0;                /* stores index of a value start */
  unsigned int end    = response.indexOf(delimiter);
                                          /* stores index of a value end */
  /* iterate over variable list */
  while (itr.hasNext()) {
    const QString &key = itr.next();      /* current variable name */
    
    /* end will be -1 if the indexOf returns -1 */
    if (end != -1) {
      params[key] = response.mid(start, end - start);
      
      /* next iteration setup */
      start = end + 1;
      end = response.indexOf(delimiter, start);
    } else {
      /* i.e. we want to have empty values for the remaining variables */
      params[key] = "";
    }
  }
  
#ifdef HANDSHAKER_DEBUG
  VariableHashItr i(params);
  while (i.hasNext()) {
    i.next();
    qDebug() << i.key() << ":\t" << i.value();
  }
#endif
  
  return params;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** this method is called by the requestComplete SLOT and is specific for when
** the request was for a PID
**
****************************************************************************/
void Handshaker::setupReceived(const QByteArray &response)
{
  //
}

}

}


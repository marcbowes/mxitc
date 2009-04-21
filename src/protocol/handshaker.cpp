/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QUrl>
#include <QDateTime>
#include <QDebug>

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
  
  state = IDLE;
}


/****************************************************************************
**
** Author: Richard Baxter
** Author: Marc Bowes
**
** sends initialization request to MXit
**
****************************************************************************/
void Handshaker::initialize()
{
  QString timestamp = QString("%1").arg(QDateTime::currentDateTime().toTime_t());
  QUrl url("http://www.mxit.com/res/");
  http->setHost(url.host(), 80);
  
  QString query =
    QString("%1?type=challenge&getcountries=true&getlanguage=true&getimage=true&ts=%2")
    .arg(url.path())
    .arg(timestamp)
  ;

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
** sends challenge response to MXit
** variables starting with an underscore were set by a previous response
**
****************************************************************************/
void Handshaker::challenge(const QString &cellphone, const QString &captcha,
  const QString &_url, const QString &_sessionid) /* from initialization */
{
  QUrl url(_url);
  http->setHost(url.host(), 80);
  
  // FIXME: **way** more environment variables needed here
  QString query =
    QString("%1?type=getpid&sessionid=%2&ver=5.8.2&login=%3&cat=Y&chalresp=%4&cc=ZA&loc=en&brand=LPM&model=StrioClient&path=1")
    .arg(url.path())
    .arg(_sessionid)
    .arg(cellphone)
    .arg(captcha)
  ;
  
#ifdef HANDSHAKER_DEBUG
  DEBUG(url.host());
  DEBUG(query);
#endif

  state = CHALLENGING;
  currentRequest = http->get(query);
}


/****************************************************************************
**
** Author: Richard Baxter
** Author: Marc Bowes
**
** this method is called by the requestComplete SLOT
**
****************************************************************************/
void Handshaker::challengeReceived(const QByteArray &response)
{
  int error = responseError(response);
  
  if (error != 0) {                         /* No error */
    // FIXME: how to handle this?
    
    return;
  }
  
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
  VariableHash params = hashVariables(response, variables);
  
  emit outgoingVariables(params);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** extracts the error code from a response
**
****************************************************************************/
int Handshaker::responseError(const QByteArray &response,
  const QString &delimiter)
{
  return response.left(response.indexOf(delimiter)).toInt();
}


/****************************************************************************
**
** Author: Marc Bowes
**
** this method is called by the requestComplete SLOT
**
****************************************************************************/
void Handshaker::setupReceived(const QByteArray &response)
{
  /* setup */
  StringVec variables;
  VariableHash params;
  
  int error = responseError(response);
  
  if (error != 0) {                         /* No error */
    switch (error) {
      case 1:                               /* Wrong answer to captcha */
        /* Response: 1;captcha */
        
        /* variable declarations for this response type */
        variables.append("err");            /* 0 = success, else failed */
        variables.append("captcha");        /* the captcha image */
        
        /* now to assign variable values from the response */
        params = hashVariables(response, variables);
        
        emit outgoingVariables(params);
        break;
      case 2:                               /* Session expired */
        /* Response: 2;sessionid;captcha or 2;captcha */
        
        /* variable declarations for this response type */
        variables.append("err");            /* 0 = success, else failed */
        variables.append("sessionid");      /* the session ID */
        variables.append("captcha");        /* the captcha image */
        
        /* now to assign variable values from the response */
        params = hashVariables(response, variables);
        
        /* if captcha is empty, then we are re-using our sessionid */
        if (params["captcha"].isEmpty()) {
          params["captcha"] = params["sessionid"];
          params["sessionid"] = "";
        }
        
        emit outgoingVariables(params);
        break;
      case 3:                               /* Undefined */
        /* Response: 3; */
        // FIXME: how to handle this?
        break;
      case 4:                               /* Critical error */
        /* Response: 4;mxitid@domain */
        // FIXME: how to handle this?
        break;  
      case 5:                               /* Internal Error - Country code not available, select another country */
        /* Response: 5; */
        // TODO
        break;
      case 6:                               /* User isn't registered (and path=0 was specified) */
        /* Response: 6;sessionid;captcha */
        // TODO
        break;
      case 7:                               /* User is already registered (and path=1 was specified) */
        /* Response: 7;sessionid;captcha */
        // TODO
        break;
    }
    
    return;
  }
  
  /* variable declarations for this response type */
  variables.append("err");                  /* 0 = success, else failed */
  variables.append("pid");                  /* the product ID */
  variables.append("soc1");                 /* the socket connection string */
  variables.append("http1");                /* the http connection string */
  variables.append("dial");                 /* country dialing code */
  variables.append("npf");                  /* country national prefix */
  variables.append("ipf");                  /* country international prefix */
  variables.append("soc2");                 /* fallback socket connection string */
  variables.append("http2");                /* fallback http connection string */
  variables.append("keepalive");            /* socket keepalive time */
  variables.append("loginname");            /* the normalized loginname (checked against MXit database) */
  variables.append("cc");                   /* the country code of the user if already registered, otherwise the
                                             * country code passed during the request */
  variables.append("region");               /* the region of the user in the database, else the region passed during
                                             * the request */
  variables.append("isUtf8Disable");        /* whether UTF-8 should be disabled in the client */
  
  /* now to assign variable values from the response */
  params = hashVariables(response, variables);
  
  emit outgoingVariables(params);
}

}

}


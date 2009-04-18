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

Client::Client(QObject *parent) : QObject (parent)
{
    httpComm = new HttpComm();
    
    
    connect(httpComm, SIGNAL(captchaReceived(const QByteArray &)), this, SLOT(captchaReceivedClient(const QByteArray &)));
}


void Client::captchaReceivedClient(const QByteArray &captcha) {
  emit captchaReceived(captcha);
}

void Client::getLoginInfo() /*includes captach image and language/country options*/{
  httpComm->sendInitialChallenge();
}

void Client::login(QString username, QString pass, QString captchaResponse)
{
  httpComm->sendChallengeResponse(captchaResponse, username);
  
}

}













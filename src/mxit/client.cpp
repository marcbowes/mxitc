/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QUrl>
#include <QDateTime>

#include "client.h"

namespace MXit
{

/****************************************************************************
**
** Author: Richard Baxter
**
** Client constructor
**
****************************************************************************/
Client::Client()
{
  handshaker = new MXit::Protocol::Handshaker();
  
  /* when the handshaker gets a CAPTCHA */
  connect(handshaker, SIGNAL(outgoingCaptcha(const QByteArray &)),
          this, SLOT(incomingCaptcha(const QByteArray &)));
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Client deconstructor
**
****************************************************************************/
Client::~Client()
{
  delete handshaker;
}


/****************************************************************************
**
** Author: Richard Baxter
**
** this SLOT is triggered by the handshaker receiving a CAPTCHA
** the signal needs to be bounced up to this client's controller
**
****************************************************************************/
void Client::incomingCaptcha(const QByteArray &captcha)
{
  emit captchaReceived(captcha);
}


/****************************************************************************
**
** Author: Richard Baxter
**
** this method instructs the handshaker to request initial information
** includes CAPTCHA image and language/country options
**
****************************************************************************/
void Client::initialize()
{
  handshaker->initialize();
}


/****************************************************************************
**
** Author: Richard Baxter
**
** this method instructs the handshaker to request a PID from MXit
**
****************************************************************************/
void Client::requestPID(const QString &cellphone, const QString &captcha)
{
  handshaker->requestPID(cellphone, captcha);
}

}


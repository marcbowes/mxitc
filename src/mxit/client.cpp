/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QUrl>
#include <QDateTime>
#include <QDebug>

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
  
  /* variable passing */
  connect(handshaker, SIGNAL(outgoingVariables(const VariableHash &)),
    this, SLOT(incomingVariables(const VariableHash &)));
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
void Client::incomingVariables(const VariableHash &params)
{
  /* TODO: need to include an origin identifier */
  if (params.contains("captcha")) {
    emit captchaReceived(QByteArray::fromBase64(params["captcha"]));
  }
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
** Author: Marc Bowes
**
** this method instructs the handshaker to request a PID from MXit
**
****************************************************************************/
void Client::login(const QString &cellphone, const QString &password, const QString &captcha)
{
  requestPID(cellphone, captcha);
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
  handshaker->challenge(cellphone, captcha);
}

}


/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QDebug>

#include "client.h"

namespace MXit
{

/****************************************************************************
**
** Author: Richard Baxter
** Author: Marc Bowes
**
** Client constructor
**
****************************************************************************/
Client::Client()
  : state (IDLE)
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
** Author: Marc Bowes
**
** this SLOT is used by by subclients to report responses of operations
** TODO: handler needs an origin type
**
****************************************************************************/
void Client::incomingVariables(const VariableHash &params)
{
  /* unite these params with the client's variables
   *
   * == NOTE
   * QHash#unite will not override keys, rather it will insert a duplicate
   * key. Ensure that common variables (e.g. err) are properly cleaned up
   * to prevent a very long useless list building up.
   */
  variables.unite(params);
  
  /* deal with the incoming variables */
  switch (state) {
    case INITIALIZING:
      initializationComplete();
      break;
    case CHALLENGING:
      challengeComplete();
      break;
    default:
      // TODO: what here?
      break;
  }
  
  /* clean up values we don't wish to store
   *
   * == NOTE
   * QHash#remove removes ALL items with the specified key
   */
  variables.remove("err");
}


/****************************************************************************
**
** Author: Marc Bowes
** Author: Richard Baxter
**
** this method instructs the handshaker to request initial information
** see Handshaker#challengeReceived for a list of variables
**
****************************************************************************/
void Client::initialize()
{
  state = INITIALIZING;
  handshaker->initialize();
}


/****************************************************************************
**
** Author: Marc Bowes
**
** abstraction method to simply login process, see submethod calls
**
****************************************************************************/
void Client::login(const QString &cellphone, const QString &password, const QString &captcha)
{
  /* need to store password so that it can be sent after challenge is complete */
  QByteArray _password; _password.append(password); /* FIXME: easier conversion? */
  variables["_password"] = _password;
  
  /* begin challenge */
  challenge(cellphone, captcha);
}


/****************************************************************************
**
** Author: Richard Baxter
** Author: Marc Bowes
**
** this method instructs the handshaker to MXit for setup information
** a successful challenge will return a PID for use with encryption
** this is required to login
**
****************************************************************************/
void Client::challenge(const QString &cellphone, const QString &captcha)
{
  state = CHALLENGING;
  handshaker->challenge(cellphone, captcha, variables["url"], variables["sessionid"]);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** this method is called by the incomingVariables SLOT
**
****************************************************************************/
void Client::challengeComplete()
{
  state = IDLE;
  // TODO: complete login process
}


/****************************************************************************
**
** Author: Marc Bowes
**
** this method is called by the incomingVariables SLOT
**
****************************************************************************/
void Client::initializationComplete()
{
  state = IDLE;
  emit captchaReceived(QByteArray::fromBase64(variables["captcha"]));
}

}


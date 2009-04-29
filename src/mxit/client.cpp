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
  connection = new MXit::Network::Connection();
  handshaker = new MXit::Protocol::Handshaker();
  
  /* incoming packets */
  connect(connection, SIGNAL(outgoingPacket(const QByteArray &)),
    this, SLOT(incomingPacket(const QByteArray &)));
  
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
  delete connection;
  delete handshaker;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** this SLOT is triggered by the connection receiving a packet
**
****************************************************************************/
void Client::incomingPacket(const QByteArray &packet)
{
  /* error checking */
  VariableHash errorHash = MXit::Protocol::packetError(packet);
  if (errorHash["code"] != "0") {    
    emit outgoingError(errorHash["code"].toInt(), errorHash["message"]);
    return;
  }
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
      setupReceived();
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
  variables.remove("captcha");
}


/****************************************************************************
**
** Author: Marc Bowes
** Author: Richard Baxter
**
  variables.remove("err");
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
  /* FIXME: easier conversion to QByteArray from QString? */
  
  /* need to store cellphone so it can be used as "id" in packets */
  QByteArray _cellphone; _cellphone.append(cellphone);
  variables["_cellphone"] = _cellphone;
  
  /* need to store password so that it can be sent after challenge is complete */
  QByteArray _password; _password.append(password);
  variables["_password"] = _password;
  
  /* begin challenge */
  challenge(cellphone, captcha);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** this method instructs the connection to build a packet and then does some
** post-build setup based on the client's state
**
****************************************************************************/
MXit::Network::Packet* Client::buildPacket()
{
  /* get the base built by the connection */
  MXit::Network::Packet *packet = connection->buildPacket();
  
  /* HTTP/TCP setup */
  packet->setCellphone(variables["loginname"]);
  
  /* HTTP only */
  if (connection->gateway.type == MXit::Network::Gateway::HTTP) {
    static_cast<MXit::Network::Packets::HTTP*>(packet)->setSessionID(variables["sessionid"].toInt());
  }
  
  return packet;
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
** Author: Richard Baxter
**
** this method is called by the incomingVariables SLOT
**
****************************************************************************/
void Client::setupReceived()
{
  state = IDLE;
  int error = variables["err"].toInt();
  
  if (error != 0) {                         /* No error */
    /* setup */
    QByteArray captcha;
    QByteArray sessionid;
    
    switch (error) {
      case 1:                               /* Wrong answer to captcha */
        /* Response: 1;captcha */
        
        /* there are two CAPTCHA's in the variables, we need to remove the old one */
        useVariable("captcha", 0);
        
        /* reporting error to client */
        emit errorEncountered("Wrong answer to CAPTCHA");
        
        break;
      case 2:                               /* Session expired */
        /* Response: 2;sessionid;captcha or 2;captcha */
        
        /* we need to correct our stored sessionid */
        useVariable("sessionid", variables["sessionid"].isEmpty() ? 1 : 0);
        
        /* there are two captcha's in the variables, we need to remove the old one */
        useVariable("captcha", 0);
        
        /* reporting error to client */
        emit errorEncountered("Session Expired");
        
        break;
      case 3:                               /* Undefined */
        /* Response: 3; */
        // FIXME: how to handle this?
        
        emit errorEncountered("Undefined Challenge error");
        break;
      case 4:                               /* Critical error */
        /* Response: 4;mxitid@domain */
        // FIXME: how to handle this?
        
        /* reporting error to client */
        emit errorEncountered("Critical Challenge error");
        
        break;  
      case 5:                               /* Internal Error - Country code not available, select another country */
        /* Response: 5; */
        emit errorEncountered("Country Code not available");
        break;
      case 6:                               /* User isn't registered (and path=0 was specified) */
        /* Response: 6;sessionid;captcha */
        
        /* there are two sessionid's in the variables, we need to remove the old one */
        useVariable("sessionid", 0);
        
        /* there are two captcha's in the variables, we need to remove the old one */
        useVariable("captcha", 0);
        
        /* reporting error to client */
        emit errorEncountered("User is not registered");
        
        break;
      case 7:                               /* User is already registered (and path=1 was specified) */
        /* Response: 7;sessionid;captcha */
        
        /* there are two sessionid's in the variables, we need to remove the old one */
        useVariable("sessionid", 0);
        
        /* there are two captcha's in the variables, we need to remove the old one */
        useVariable("captcha", 0);
        
        /* reporting error to client */
        emit errorEncountered("User is already registered");
        
        break;
    }
    
    return;
  }
  
  /* gateway setup so the connection can connect */
  connection->addGateway(variables["soc1"]);
  connection->addGateway(variables["http1"]);
  connection->addGateway(variables["soc2"]);
  connection->addGateway(variables["http2"]);
  
  /* send off a login packet */
  MXit::Network::Packet *packet = buildPacket();
  MXit::Protocol::Handlers::Login login;
  login.build(packet, variables);
  connection->sendPacket(*packet);
  delete packet;
  
  /* cleanup */
  variables.remove("sessionid");
  variables.remove("_cellphone");
  variables.remove("_password");
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


/****************************************************************************
**
** Author: Marc Bowes
**
** uniquely assigns a variable value against an index value
** does nothing if the variable doesn't exist
**
****************************************************************************/
void Client::useVariable(const QString &variable, unsigned int index)
{
  /* find most recent key */
  VariableHashConstItr itr = variables.find(variable);
  if (itr == variables.end()) {                   /* no references found */
    return;
  }
  
  /* try iterate up to index */
  const QByteArray *ref;
  unsigned int walk = 0;
  while (itr != variables.end() && itr.key() == variable && walk < index) {
    itr++; walk++;
    ref = &itr.value();
  }
  
  /* assign *only* the found value */
  QByteArray value = *ref;                        /* make a duplication of desired reference */
  variables.remove(variable);                     /* remove all copies of the variable */
  variables[variable] = value;                    /* now assign our unique value */
}

}


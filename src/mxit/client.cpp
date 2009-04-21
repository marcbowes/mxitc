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
  packet->setCellphone(variables["_cellphone"]);
  
  /* HTTP only */
  if (connection->gateway.type == MXit::Network::Gateway::HTTP) {
    static_cast<MXit::Network::HTTPPacket*>(packet)->setSessionID(variables["sessionid"].toInt());
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
  
  connection->addGateway(variables["soc1"]);
  connection->addGateway(variables["http1"]);
  connection->addGateway(variables["soc2"]);
  connection->addGateway(variables["http2"]);
  
  // TODO probably should put code below this line into it's own method (?)
  
  // (for login code below) Author: Richard Baxer
  
  // TODO see page 7 of mxit open protocol, we need to still put in that ["cr"=splashName0 \1 splashName1 \1 ... \1 splashNameN]
  
  Network::Packet * packetToSend = buildPacket();
  
  packetToSend->setCellphone(variables["_cellphone"]);
  packetToSend->setCommand("1");
  
  /* see definitions on pg 7 of mxit open protocol*/
  (*packetToSend) << variables["_password"] /* password */
               << "MXITC-0.0-Y-Generic_PC"          /* version == distributorCode-releaseVersion-archSeries-platform - see pg 7 FIXME ... i think what I've made this should be alright*/ 
               << "0"                               /* getContacts - FIXME just setting to 0 for 'don't return contacts', should be 0|1 */
               << "w=640;h=480;c=65536;utf8=true"   /* capabilities - FIXME just filling in some values - see pg 8*/
               << "distribution_code"               /* dc - distribution code FIXME wtf is this supposed to be - see pg 7*/
               << "0x0"                             /* features - FIXME 0x0 == no features */
               << variables["defaultDialingCode"]   /* dialingCode - FIXME should we be setting this to something? */
               << "en"                              /* locale - FIXME don't know what to set this to, just set to en*/
               ;
  connection->enqueue(*packetToSend);
  delete packetToSend;
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


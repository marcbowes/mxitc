/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "protocol/packet_header.h"

#include "client.h"

namespace MXit
{

/****************************************************************************
        __                                _ ____    
   ____/ /__ ____ ___   ___ ___  ___ ____(_) _(_)___
  / __/ / _ `(_-<(_-<  (_-</ _ \/ -_) __/ / _/ / __/
  \__/_/\_,_/___/___/ /___/ .__/\__/\__/_/_//_/\__/ 
                         /_/                        

****************************************************************************/


/****************************************************************************
**
** Author: Richard Baxter
** Author: Marc Bowes
**
** Client constructor
**
****************************************************************************/
Client::Client()
  : state (IDLE), variables(), keepAliveTimer()
{
  connection = new MXit::Network::Connection();
  handshaker = new MXit::Protocol::Handshaker();
  
  /* incoming packets */
  connect(connection, SIGNAL(outgoingPacket(const QByteArray &)),
    this, SLOT(incomingPacket(const QByteArray &)));
  
  /* variable passing */
  connect(handshaker, SIGNAL(outgoingVariables(const VariableHash &)),
    this, SLOT(incomingVariables(const VariableHash &)));
  
  /* error bubbling between connection and ui */
  connect(connection, SIGNAL(outgoingError(const QString &)),
    this, SLOT(incomingError(const QString &)));
    
  /* keep alive */
  connect(&keepAliveTimer, SIGNAL(timeout()), this, SLOT(keepAlive()));
  keepAliveTimer.setSingleShot(true);
  
  /* httpPoll timer FIXME: use a variable for timing*/
  connect(&httpPollTimer, SIGNAL(timeout()), this, SLOT(httpPoll()));
  httpPollTimer.start(15000);
  
  /* create handlers */
  using namespace MXit::Protocol::Handlers;
  /* 01 */ handlers["login"]                      = new Login();
  /* 02 */ handlers["logout"]                     = new Logout();
  /* 03 */ handlers["getcontacts"]                = new GetContacts();
  /* 05 */ handlers["updatecontactinfo"]          = new UpdateContactInfo();
  /* 06 */ handlers["subscribetoanewcontact"]     = new SubscribeToANewContact();
  /* 08 */ handlers["removecontact"]              = new RemoveContact();
  /* 09 */ handlers["getnewmessages"]             = new GetNewMessages();
  /* 10 */ handlers["sendnewmessage"]             = new SendNewMessage();
  /* 11 */ handlers["register"]                   = new Register();
  /* 12 */ handlers["updateprofile"]              = new UpdateProfile();
  /* 17 */ handlers["polldifference"]             = new PollDifference();
  /* 27 */ handlers["getmultimediamessage"]       = new GetMultimediaMessage();
  /* 32 */ handlers["setshownpresenceandstatus"]  = new SetShownPresenceAndStatus();
  /* 33 */ handlers["blocksubscription"]          = new BlockSubscription();
  /* 43 */ handlers["loginkick"]                  = new LoginKick();
  /* 51 */ handlers["getnewsubscription"]         = new GetNewSubscription();
  /* 52 */ handlers["allowsubscription"]          = new AllowSubscription();
  /* 55 */ handlers["denysubscription"]           = new DenySubscription();
  /* 1k */ handlers["keepalive"]                  = new KeepAlive();
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Csrclient deconstructor
**
****************************************************************************/
Client::~Client()
{
  delete connection;
  delete handshaker;
  
  /* free handlers */
  Q_FOREACH(MXit::Protocol::Handler *h, handlers)
    delete h;
}


/****************************************************************************
                __   ___           __     __    
     ___  __ __/ /  / (_)___  ___ / /__  / /____
    / _ \/ // / _ \/ / / __/ (_-</ / _ \/ __(_-<
   / .__/\_,_/_.__/_/_/\__/ /___/_/\___/\__/___/
  /_/                                           

****************************************************************************/


/****************************************************************************
**
** Author: Marc Bowes
**
** Passes parameters onto a packet handler and transmits result
**
****************************************************************************/
void Client::addContact(const QString &group, const QString &contactAddress, const QString &nickname,
    Protocol::Enumerables::Contact::Type type, const QString &message)
{
  /* packet variables */
  VariableHash subscriptionVariables;
  subscriptionVariables["group"]      = group.toUtf8();
  subscriptionVariables["contact_loginname"]
                                      = contactAddress.toUtf8();
  subscriptionVariables["nickname"]   = nickname.toUtf8();
  subscriptionVariables["type"]       = QString("%1").arg(type).toUtf8();
  subscriptionVariables["msg"]        = message.toUtf8();
  
  sendPacket("subscribetoanewcontact", subscriptionVariables);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Passes parameters onto a packet handler and transmits result
**
****************************************************************************/
void Client::allowSubscription(const QString &contactAddress, const QString &group,
  const QString &nickname)
{
  /* packet variables */
  VariableHash subscriptionVariables;
  subscriptionVariables["contactAddress"]
                           = contactAddress.toUtf8();
  subscriptionVariables["group"]
                          = group.toUtf8();
  subscriptionVariables["nickname"]
                          = nickname.toUtf8();
  
  sendPacket("allowsubscription", subscriptionVariables);
}


/****************************************************************************
**
** Author: Marc Bowes
** Author: Richard Baxter
**
** same as login, but skips the handshaking phase
**
****************************************************************************/
void Client::authenticate(const VariableHash &settings)
{
  variables.unite(settings);
  emit outgoingVariables(variables);
  
  connection->setGateway(variables["soc1"]);
  connection->open(getPacket("login"));
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Passes parameters onto a packet handler and transmits result
** If block is set to true a block request will be sent (prevents future
** subscription attempts)
**
****************************************************************************/
void Client::denySubscription(const QString &contactAddress, bool block)
{
  /* packet variables */
  VariableHash subscriptionVariables;
  subscriptionVariables["contactAddress"]
                           = contactAddress.toUtf8();
  
  if (block)
    sendPacket("blocksubscription", subscriptionVariables);
  else
    sendPacket("denysubscription", subscriptionVariables);
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
****************************************************************************/
void Client::getContacts()
{
  /* FIXME: stub */
}


/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/
void Client::getNewMessages()
{
  /* FIXME: stub */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** abstraction method to simply login process, see submethod calls
**
****************************************************************************/
void Client::login(const QString &cellphone, const QString &password, const QString &captcha,
  const VariableHash &settings)
{
  /* need to store cellphone so it can be used as "id" in packets */
  variables["_cellphone"] = cellphone.toLatin1();
  
  /* need to store password so that it can be sent after challenge is complete */
  variables["_password"] = password.toLatin1();
  
  /* merge in settings */
  variables.unite(settings);
  
  /* begin challenge */
  challenge(captcha);
  
  emit outgoingVariables(variables);
}


/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/
void Client::pollDifference()
{
  /* FIXME: stub */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Passes parameters onto a packet handler and transmits result
**
****************************************************************************/
void Client::removeContact(const QString &contactAddress)
{
  /* packet variables */
  VariableHash removeVariables;
  removeVariables["contactAddress"]   = contactAddress.toUtf8();
  
  sendPacket("removecontact", removeVariables);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** sets the gateway, and deals with reconnecting
**
****************************************************************************/
void Client::setGateway(const QString &connectionString)
{
  if (connection->getState() != MXit::Network::Connection::DISCONNECTED) {
    sendPacket("logout");
    connection->close();
  }
  connection->setGateway(connectionString);
  connection->open(getPacket("login"));
}


/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/
void Client::setShownPresenceAndStatus()
{
  /* FIXME: stub */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Passes parameters onto a packet handler and transmits result
**
****************************************************************************/
void Client::sendMessage(const QString &contactAddress, const QString &message, Protocol::Enumerables::Message::Type type, unsigned int flags)
{
  /* packet variables */
  VariableHash messageVariables;
  messageVariables["contactAddress"]  = contactAddress.toUtf8();
  messageVariables["message"]         = message.toUtf8();
  messageVariables["type"]            = QString("%1").arg(type).toUtf8();
  messageVariables["flags"]           = QString("%1").arg(flags).toUtf8();
  
  sendPacket("sendnewmessage", messageVariables);
}


/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/
void Client::signup()
{
  /* FIXME: stub */
}


/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/
void Client::updateContactInfo()
{
  /* FIXME: stub */
}


/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/
void Client::updateProfile(const QString &pin, const QString &name, bool hiddenLoginname,
    const QDate &dateOfBirth, const QString &gender)
{
  VariableHash profileVariables;
  profileVariables["pin"]             = pin.toUtf8();
  profileVariables["name"]            = name.toUtf8();
  profileVariables["hiddenLoginname"] = hiddenLoginname ? "1" : "0";
  profileVariables["dateOfBirth"]     = dateOfBirth.toString(Qt::ISODate).toUtf8();
  profileVariables["gender"]          = gender.toLower() == "male" ? "1" : "0";
  
  sendPacket("updateprofile", profileVariables);
}


/****************************************************************************
                __   ___                 __  __           __  
     ___  __ __/ /  / (_)___  __ _  ___ / /_/ /  ___  ___/ /__
    / _ \/ // / _ \/ / / __/ /  ' \/ -_) __/ _ \/ _ \/ _  (_-<
   / .__/\_,_/_.__/_/_/\__/ /_/_/_/\__/\__/_//_/\___/\_,_/___/
  /_/                                                         

****************************************************************************/


/****************************************************************************
**
** Author: Marc Bowes
**
** returns (most recent) value of a variable
**
****************************************************************************/
QByteArray Client::variableValue(const QString &name)
{
  return variables[name];
}


/****************************************************************************
               _           __            __     __    
     ___  ____(_)  _____ _/ /____   ___ / /__  / /____
    / _ \/ __/ / |/ / _ `/ __/ -_) (_-</ / _ \/ __(_-<
   / .__/_/ /_/|___/\_,_/\__/\__/ /___/_/\___/\__/___/
  /_/                                                 

/***************************************************************************/


/****************************************************************************
**
** Author: Marc Bowes
**
** this SLOT is triggered by the connection emitting an error
**
****************************************************************************/
void Client::incomingError(const QString &error)
{
  emit outgoingConnectionError(error);
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
  VariableHash packetHeader = MXit::Protocol::packetHeader(packet);
  if (packetHeader["errorCode"] != "0") {    
    emit outgoingError(packetHeader["errorCode"].toInt(), packetHeader["errorMessage"]);
    return;
  }
  
  MXit::Protocol::Handler *handler = handlerFor(packetHeader["command"]);
  
  /* deal with unknown packets */
  if (!handler) {
    emit outgoingError(99, QString("Unkown packet handler for command %1").arg(QString(packetHeader["command"])));
    return;
  }
  
  /* pass on to handler */
  VariableHash handledPacket = handler->handle(packet);
  variables.unite(handledPacket);
  
  /* post packet-level handling */
  switch (packetHeader["command"].toUInt()) {
    case LOGIN:
      emit outgoingAction(LOGGED_IN);

      /* variable scrubbing */
      useVariable("loginname", 0);
      
      /* need to send presence to remain online */
      variables["show"]   = "1";        /* online */
      variables["status"] = "mxitc";
      sendPacket("setshownpresenceandstatus");
      break;
    case LOGOUT:
      connection->close();
      
      emit outgoingAction(LOGGED_OUT);
      break;
    case GETCONTACTS:
      emit outgoingAction(CONTACTS_RECEIVED);

      /* variable scrubbing */
      variables.remove("contacts");
      break;
    case GETNEWMESSAGES:
      emit outgoingAction(MESSAGE_RECEIVED);

      /* variable scrubbing */
      variables.remove("contactData");
      variables.remove("message");
      variables.remove("contactAddress");
      variables.remove("dateTime");
      variables.remove("type");
      variables.remove("id");
      variables.remove("flags");
      break;
    case GETNEWSUBSCRIPTION:
      emit outgoingAction(SUBSCRIPTIONS_RECEIVED);

      /* variable scrubbing */
      variables.remove("contacts");
      break;
    case LOGINKICK:
      sendPacket("login");
      break;
  }
  
  /* global scrubbing */
  variables.remove("ln");
  variables.remove("command");
  variables.remove("error");
  
  emit outgoingVariables(variables);
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
  
  emit outgoingVariables(variables);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Called every so often by keepAliveTimer
** sends packet 1000 if 15 minutes of no activity has elapsed to prevent
** MXit dropping the connection
**
****************************************************************************/
void Client::keepAlive()
{
  sendPacket("keepalive");
}


/****************************************************************************
**
** Author: Tim Sjoberg
**
** Called every so often by keepAliveTimer
** sends packet 17 to get new info from mxit if on http
**
****************************************************************************/
void Client::httpPoll()
{
  if (connection->isHTTP()) {
    sendPacket("polldifference");
  }
}


/****************************************************************************
               _           __                  __  __           __  
     ___  ____(_)  _____ _/ /____   __ _  ___ / /_/ /  ___  ___/ /__
    / _ \/ __/ / |/ / _ `/ __/ -_) /  ' \/ -_) __/ _ \/ _ \/ _  (_-<
   / .__/_/ /_/|___/\_,_/\__/\__/ /_/_/_/\__/\__/_//_/\___/\_,_/___/
  /_/                                                               

****************************************************************************/


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
  if (connection->isHTTP()) {
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
void Client::challenge(const QString &captcha)
{
  state = CHALLENGING;
  handshaker->challenge(captcha, variables);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** convenience method for packets using class variables
**
****************************************************************************/
MXit::Network::Packet* Client::getPacket(const QString &handler)
{
  return getPacket(handler, variables);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** builds a packet from the specified handler
**
****************************************************************************/
MXit::Network::Packet* Client::getPacket(const QString &handler, VariableHash &packetVariables)
{
  MXit::Network::Packet *packet = buildPacket();
  handlers[handler]->build(packet, packetVariables);
  return packet;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** finds a handler by command
**
****************************************************************************/
MXit::Protocol::Handler* Client::handlerFor(const QByteArray &command)
{
  unsigned int c = command.toUInt();
  Q_FOREACH(MXit::Protocol::Handler *h, handlers) {
    if (h->command == c) return h;
  }
  
  return NULL; /* no packet found */
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
  emit environmentReady();
}


/****************************************************************************
**
** Author: Marc Bowes
**
** convenience method for packets using class variables
**
****************************************************************************/
void Client::sendPacket(const QString &handler)
{
  sendPacket(handler, variables);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** builds and sends a packet of the specified type from a variable hash
**
****************************************************************************/
void Client::sendPacket(const QString &handler, VariableHash &packetVariables)
{
  connection->sendPacket(getPacket(handler, packetVariables));
  keepAliveTimer.start(1000 * 60 * 15); /* every 15 minutes */
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
  
  connection->setGateway(variables["soc1"]);
  connection->open(getPacket("login"));
  
  /* cleanup */
  variables.remove("sessionid");
  variables.remove("_cellphone");
  variables.remove("_password");
  useVariable("cc", 0);
  useVariable("locale", 0);
  
  emit outgoingVariables(variables);
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
  const QByteArray *ref = &itr.value(); /* Initialise this, index was set to 0 and ref was NULL*/
  unsigned int walk = 0;
  while (itr != variables.end() && itr.key() == variable && walk < index) {
    itr++; walk++;
    ref = &itr.value();
  }
  
  /* assign *only* the found value */
  QByteArray value = *ref;                        /* make a duplication of desired reference */
  variables.remove(variable);                     /* remove all copies of the variable */
  variables[variable] = value;                    /* now assign our unique value */
  
  emit outgoingVariables(variables);
}

}


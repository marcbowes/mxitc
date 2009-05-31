/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "protocol/packet_header.h"
#include "protocol/enumerables/chunked_data.h"

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
** Author: Marc Bowes
**
** Client constructor
**
****************************************************************************/
Client::Client()
  : state (IDLE), variables(), keepAliveTimer(), registerAfterChallenge(false)
{
  connection = new MXit::Network::Connection();
  handshaker = new MXit::Protocol::Handshaker();

  /* variable passing */
  connect(handshaker, SIGNAL(outgoingVariables(const VariableHash &)),
    this, SLOT(incomingVariables(const VariableHash &)));

  /* incoming packets */
  connect(connection, SIGNAL(outgoingPacket(const QByteArray &)),
    this, SLOT(incomingPacket(QByteArray)));

  /* incoming states */
  connect(connection, SIGNAL(outgoingState(Network::Connection::State)),
    this, SIGNAL(outgoingConnectionState(Network::Connection::State)));

  /* error bubbling between connection and ui */
  connect(connection, SIGNAL(outgoingError(const QString &)),
    this, SLOT(incomingError(const QString &)));

  /* keep alive */
  connect(&keepAliveTimer, SIGNAL(timeout()), this, SLOT(keepAlive()));
  keepAliveTimer.setSingleShot(true);

  /* poll difference */
  connect(&pollTimer, SIGNAL(timeout()), this, SLOT(pollDifference()));

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
  /* 26 */ handlers["getprofile"]                 = new GetProfile();
  /* 27 */ handlers["getmultimediamessage"]       = new GetMultimediaMessage();
  /* 29 */ handlers["renamegroup"]                = new RenameGroup();
  /* 30 */ handlers["removegroup"]                = new RemoveGroup();
  /* 31 */ handlers["splashscreenclickthrough"]   = new SplashScreenClickthrough();
  /* 32 */ handlers["setshownpresenceandstatus"]  = new SetShownPresenceAndStatus();
  /* 33 */ handlers["blocksubscription"]          = new BlockSubscription();
  /* 34 */ handlers["sendmessagetogroup"]         = new SendMessageToGroup();
  /* 41 */ handlers["setmood"]                    = new SetMood();
  /* 43 */ handlers["loginkick"]                  = new LoginKick();
  /* 44 */ handlers["createnewgroupchat"]         = new CreateNewGroupchat();
  /* 45 */ handlers["addnewgroupchatmember"]      = new AddNewGroupchatMember();
  /* 51 */ handlers["getnewsubscription"]         = new GetNewSubscription();
  /* 52 */ handlers["allowsubscription"]          = new AllowSubscription();
  /* 54 */ handlers["registergateway"]            = new RegisterGateway();
  /* 55 */ handlers["denysubscription"]           = new DenySubscription();
  /* 56 */ handlers["unregistergateway"]          = new UnregisterGateway();
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
  emit environmentReady();
  emit outgoingVariables(variables);

  connection->setGateway(variables["soc1"], "", 0, "", ""); /*FIXME, get these from settings!*/
  connection->open(getPacket("login"));
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Creates a new Group Chat
**
****************************************************************************/
void Client::createNewGroupChat(const QString &roomName, const ContactList &contacts)
{
  /* contact addresses */
  QStringList contactList;
  Q_FOREACH(const Contact *contact, contacts)
    if (contact->notBot())
      contactList << contact->contactAddress;

  /* packet variables */
  VariableHash groupVariables;
  groupVariables["roomname"] = roomName.toUtf8();
  groupVariables["numContacts"] = QByteArray::number(contacts.size());
  groupVariables["contactList"] = contactList.join("\1").toUtf8();

  sendPacket("createnewgroupchat", groupVariables);
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
** Author: Tim Sjoberg
** Author: Marc Bowes
**
** Deals with commands/markup by messaging-out the text clicked on
** Assumes the URL is properly formatted
**
****************************************************************************/
void Client::linkClicked(const QUrl &url)
{
  QString contactAddress, text, urlString;
  quint16 idx;
  Protocol::Enumerables::Message::Type type;

  /* strip mxit:/ */
  urlString = url.toString(QUrl::RemoveScheme);
  
  /* bogus URL, eg. message from self */
  if (!urlString.startsWith("//"))
    return;
  
  /* remove the // */
  urlString.remove(0, 2);
  
  /* find contactAddress (before first /) */
  idx = urlString.indexOf('/');
  contactAddress = urlString.left(idx);
  /* remove everything until after the first / */
  urlString.remove(0, idx + 1);
  
  /* find type (after last /) */
  idx = urlString.lastIndexOf('/');
  type = Protocol::Enumerables::Message::Type(
    urlString.right(urlString.length() - idx - 1).toUInt());
  /* remove everything after the last / */
  urlString.truncate(idx);
  
  text = urlString; /* readability */

  if (type == Protocol::Enumerables::Message::Chat) {
    /* reply with the text to the contact */
    /* essentially, $this$ = message the contact with "this" */
    sendMessage(contactAddress, text, type, Protocol::Enumerables::Message::MayContainMarkup);
  } else if (type == Protocol::Enumerables::Message::Command) {
    /* format reply: FIXME: hack for reply only */
    QStringList tempData(text.split("|"));
    VariableHash tempHash;

    Q_FOREACH (const QString &option, tempData) {
      int equalsPos = option.indexOf("=");
      tempHash[option.mid(0, equalsPos)] = option.mid(equalsPos + 1).toUtf8();
    }

    if (tempHash["type"] == "reply") {
      text = QString("::type=reply|res=%1|error=0:").arg(QString(tempHash["replymsg"]));
      /* sendMessage(contactAddress, text, type, 0);/* /* FIXME: MXit don't implement their own protocol?? */
      sendMessage(contactAddress, tempHash["replymsg"], type, 0);
    }
  }
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
** Sends a pollDifference packet, only intended for use with HTTP.
**
****************************************************************************/
void Client::pollDifference()
{
  if (connection->isHTTP()) {
    sendPacket("polldifference");
    pollTimer.start(variables["polltimer"].toUInt());
  }
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
** Author: Tim Sjoberg
**
** Passes parameters onto a packet handler and transmits result
**
****************************************************************************/
void Client::sendFile(QFile &file, const ContactList &contacts)
{
  //type
  VariableHash fileVariables;
  fileVariables["type"] = QByteArray::number(MXit::Protocol::Enumerables::ChunkedData::SendFileDirect);
  
  if (!file.isOpen())
    file.open(QIODevice::ReadOnly);
  
  //size
  int size = file.size();
  QByteArray temp = QByteArray::number(size, 16);
  while (temp.size() < (4*2))
    temp.prepend("0");
  fileVariables["size"] = QByteArray::fromHex(temp);
  
  //numContacts
  int numContacts = contacts.size();
  temp = QByteArray::number(numContacts, 16);
  while (temp.size() < (2*2))
    temp.prepend("0");
  fileVariables["numContacts"] = temp;
  
  //list of contacts
  temp.clear();
  Q_FOREACH(const Contact *contact, contacts) {
    QString contactAddress = contact->contactAddress;
    int length = contactAddress.length();
    QByteArray byteLength = QByteArray::number(length, 16);
    while (byteLength.size() < (2*2))
      byteLength.prepend("0");
    temp.append(QByteArray::fromHex(byteLength));
    temp.append(contactAddress);
  }
  fileVariables["addresses"] = temp;
  
  //filename
  QString fileName = file.fileName();
  fileVariables["name"] = fileName.toUtf8();
  
  //FIXME: actually do mimetypes
  if ((fileName.endsWith(".png")) || (fileName.endsWith(".x-png"))) {
    fileVariables["mimetype"] = "image/png";
  } else if ((fileName.endsWith(".jpe")) || (fileName.endsWith(".jpg")) || (fileName.endsWith(".jpeg")) || (fileName.endsWith(".jfif"))) {
    fileVariables["mimetype"] = "image/jpeg";
  } else if (fileName.endsWith(".svg")) {
    fileVariables["mimetype"] = "image/svg+xml";
  } else if (fileName.endsWith(".svgz")) { //FIXME this shouldnt be image/svgz+xml, but mxit wants it that way afaik
    fileVariables["mimetype"] = "image/svgz+xml";
  } else if (fileName.endsWith(".wav")) {
    fileVariables["mimetype"] = "audio/wav";
  } else if ((fileName.endsWith(".midi")) || (fileName.endsWith(".kar")) || (fileName.endsWith(".mid"))) {
    fileVariables["mimetype"] = "audio/midi";//FIXME not finished
  } else {
    fileVariables["mimetype"] = "application/octet-stream";
  }
  
  //description TODO: maybe allow for a description
  fileVariables["description"] = "";
  
  //FIXME: implement crc
  temp = QByteArray::number(0, 16);
  while (temp.size() < (4*2))
    temp.prepend("0");
  fileVariables["crc"] = temp;
  
  //actual file data
  fileVariables["bytes"] = file.readAll();
  
  sendPacket("getmultimediamessage", fileVariables);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** sets the gateway, and deals with reconnecting
**
****************************************************************************/
void Client::setGateway(const QString &connectionString, const QString &httpHost, quint16 port,
  const QString &username, const QString &password)
{
  if (connection->getState() != MXit::Network::Connection::DISCONNECTED) {
    sendPacket("logout");
    connection->close();
    emit outgoingAction(LOGGED_OUT);
  }
  connection->setGateway(connectionString, httpHost, port, username, password);
  connection->open(getPacket("login"));
}


/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/
void Client::setMood(Protocol::Enumerables::Contact::Mood mood)
{
  /* packet variables */
  VariableHash moodVariables;
  moodVariables["mood"] = QByteArray::number(mood);

  sendPacket("setmood", moodVariables);
}


/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/
void Client::setShownPresenceAndStatus(Protocol::Enumerables::Contact::Presence presence,
  const QString &status)
{
  /* packet variables */
  VariableHash presenceVariables;
  presenceVariables["show"]   = QByteArray::number(presence);
  presenceVariables["status"] = status.toUtf8();

  sendPacket("setshownpresenceandstatus", presenceVariables);
}


/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/
void Client::sendGroupMessage(const QString &group, const ContactList &contacts,
    const QString &message, Protocol::Enumerables::Message::Type type, unsigned int flags)
{
  /* contact addresses */
  QStringList contactList;
  Q_FOREACH(const Contact *contact, contacts)
    if (contact->type != Protocol::Enumerables::Contact::Bot)
      contactList << contact->contactAddress;

  /* packet variables */
  VariableHash groupVariables;
  groupVariables["group"] = group.toUtf8();
  groupVariables["numContacts"] = QByteArray::number(contacts.size());
  groupVariables["contactList"] = contactList.join("\1").toUtf8();
  groupVariables["type"] = QByteArray::number(type);
  groupVariables["flags"] = QByteArray::number(flags);

  sendPacket("sendmessagetogroup", groupVariables);
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
void Client::signup(const QString &cellphone, const QString &password, const QString &captcha,
    const VariableHash &settings)
{
  /* need to store cellphone so it can be used as "id" in packets */
  variables["_cellphone"] = cellphone.toLatin1();

  /* need to store password so that it can be sent after challenge is complete */
  variables["_password"] = password.toLatin1();

  /* merge in settings */
  variables.unite(settings);

  /* begin challenge */
  registerAfterChallenge = true;
  challenge(captcha);

  emit outgoingVariables(variables);
}


/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/
void Client::updateContactInfo(const QString &group, const QString &contactAddress,
    const QString &nickname)
{
  /* packet variables */
  VariableHash updateVariables;
  updateVariables["contactAddress"]  = contactAddress.toUtf8();
  updateVariables["group"]           = group.toUtf8();
  updateVariables["nickname"]        = nickname.toUtf8();

  sendPacket("updatecontactinfo", updateVariables);
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
void Client::incomingPacket(QByteArray packet)
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

  QByteArray handle; /* for #27 */

  /* post packet-level handling */
  switch (packetHeader["command"].toUInt()) {
    case LOGIN:
    case REGISTER:
      emit outgoingAction(LOGGED_IN);

      /* variable scrubbing */
      useVariable("loginname", 0);

      /* send custom online presence */
      setShownPresenceAndStatus(Protocol::Enumerables::Contact::Online, "mxitc");

      if (connection->isHTTP()) {
        /* start the first pollDifference */
        if (variables["polltimer"].toUInt() == 0)
          variables["polltimer"] = QByteArray::number(15 * 1000); /* default */
        pollTimer.start(variables["polltimer"].toUInt());
      }
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
    case GETMULTIMEDIAMESSAGE:
      emit outgoingAction(MULTIMEDIA_RECEIVED);

      /* variable scrubbing */
      handle = variables.value("handle");
      variables.remove(handle + "_anchor");
      variables.remove(handle + "_bgColour");
      variables.remove(handle + "_image");
      variables.remove(handle + "_timeToShow");
      variables.remove(handle + "_type");
      variables.remove("handle");
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
    static_cast<MXit::Network::Packets::HTTP*>(packet)->setSessionID(variables["sesid"].toInt());
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
  handshaker->challenge(captcha, variables, registerAfterChallenge);
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
  qDebug() << "in setupReceived -- " << error;

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
        emit outgoingError(error, "Wrong answer to CAPTCHA");

        break;
      case 2:                               /* Session expired */
        /* Response: 2;sessionid;captcha or 2;captcha */

        /* we need to correct our stored sessionid */
        useVariable("sessionid", variables["sessionid"].isEmpty() ? 1 : 0);

        /* there are two captcha's in the variables, we need to remove the old one */
        useVariable("captcha", 0);

        /* reporting error to client */
        emit outgoingError(error, "Session Expired");

        break;
      case 3:                               /* Undefined */
        /* Response: 3; */
        // FIXME: how to handle this?

        emit outgoingError(error, "Undefined Challenge error");
        break;
      case 4:                               /* Critical error */
        /* Response: 4;mxitid@domain */
        // FIXME: how to handle this?

        /* reporting error to client */
        emit outgoingError(error, "Critical Challenge error");

        break;
      case 5:                               /* Internal Error - Country code not available, select another country */
        /* Response: 5; */
        emit outgoingError(error, "Country Code not available");
        break;
      case 6:                               /* User isn't registered (and path=0 was specified) */
        /* Response: 6;sessionid;captcha */

        /* there are two sessionid's in the variables, we need to remove the old one */
        useVariable("sessionid", 0);

        /* there are two captcha's in the variables, we need to remove the old one */
        useVariable("captcha", 0);

        /* reporting error to client */
        emit outgoingError(error, "User is not registered");

        break;
      case 7:                               /* User is already registered (and path=1 was specified) */
        /* Response: 7;sessionid;captcha */

        /* there are two sessionid's in the variables, we need to remove the old one */
        useVariable("sessionid", 0);

        /* there are two captcha's in the variables, we need to remove the old one */
        useVariable("captcha", 0);

        /* reporting error to client */
        emit outgoingError(error, "User is already registered");

        break;
    }

    return;
  }

  connection->setGateway(variables["soc1"], "", 0, "", "");
  if (!registerAfterChallenge)
    connection->open(getPacket("login"));
  else {
    qDebug() << "registering?";
    connection->open(getPacket("register"));
    registerAfterChallenge = false;
  }

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


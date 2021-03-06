/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
** Author: Richard Baxter, 2009
** Author: Timothy Sjoberg, 2009
**
** MXit::Client is an abstraction layer, providing functionality rather than
** implementation. Implementation is found at lower levels, e.g. networking
**
****************************************************************************/

#ifndef __MXIT_CLIENT_H__
#define __MXIT_CLIENT_H__

#include <QDate>
#include <QHash>
#include <QTimer>
#include <QUrl>
#include <QFile>

#include "common/actions.h"

#include "network/connection.h"

#include "protocol/aes.h"
#include "protocol/handshaker.h"
#include "protocol/handlers/include.list"
#include "protocol/enumerables/contact.h"
#include "protocol/enumerables/message.h"

#include "contact.h"

namespace MXit
{

class Client : public QObject
{
  Q_OBJECT

  public:         /* class specific */

  Client();
  ~Client();

  enum Status {
    IDLE,
    INITIALIZING,
    CHALLENGING
  };

  signals:

  void environmentReady();
  void outgoingAction(Action);
  void outgoingConnectionError(const QString &message);
  void outgoingConnectionState(Network::Connection::State);
  void outgoingMessage(const QString & contactAddress, const QString & message);
  void outgoingError(int code, const QString &message);
  void outgoingVariables(const VariableHash&);

  public slots:

  void addContact(const QString &group, const QString &contactAddress, const QString &nickname,
    Protocol::Enumerables::Contact::Type type, const QString &message);
  void allowSubscription(const QString &contactAddress, const QString &group,
    const QString &nickname);
  void authenticate(const VariableHash &settings);
  void createNewGroupChat(const QString &roomName, const ContactList &contacts);
  void denySubscription(const QString &contactAddress, bool block=false);
  void initialize();
  void getContacts();
  void getNewMessages();
  void linkClicked(const QUrl &url);
  void login(const QString &cellphone, const QString &password, const QString &captcha,
    const VariableHash &settings);
  void logout();
  void pollDifference();
  void registerGateway(const QString &username, const QString &Password,
    Protocol::Enumerables::Contact::Type type);
  void removeContact(const QString &contactAddress);
  void sendFile(QFile &file, const ContactList &contacts);
  void setGateway(const QString &connectionString, const QString &proxyHost, quint16 proxyPort,
    const QString &username, const QString &password);
  void setMood(Protocol::Enumerables::Contact::Mood mood);
  void setShownPresenceAndStatus(Protocol::Enumerables::Contact::Presence presence,
    const QString &status = "");
  void sendGroupMessage(const QString &group, const ContactList &contacts,
    const QString &message, Protocol::Enumerables::Message::Type type, unsigned int flags);
  void sendMessage(const QString &contactAddress, const QString &message,
    Protocol::Enumerables::Message::Type, unsigned int flags);
  void signup(const QString &cellphone, const QString &password, const QString &captcha,
    const VariableHash &settings);
  void unregisterGateway(const QString &username, Protocol::Enumerables::Contact::Type type);
  void updateContactInfo(const QString &group, const QString &contactAddress,
    const QString &nickname);
  void updateProfile(const QString &pin, const QString &name, bool hiddenLoginname,
    const QDate &dateOfBirth, const QString &gender);

  public:         /* methods */

  QByteArray variableValue(const QString &name);

  private slots:

  void incomingError(const QString &error);
  void incomingPacket(QByteArray packet);
  void incomingVariables(const VariableHash &variables);
  void keepAlive();

  private:        /* methods */

  MXit::Network::Packet* buildPacket();
  void challenge(const QString &captcha);
  MXit::Network::Packet* getPacket(const QString &handler);
  MXit::Network::Packet* getPacket(const QString &handler, VariableHash &packetVariables);
  MXit::Protocol::Handler* handlerFor(const QByteArray &command);
  void initializationComplete();
  void sendPacket(const QString &handler);
  void sendPacket(const QString &handler, VariableHash &packetVariables);
  void setupReceived();
  void useVariable(const QString &variable, unsigned int index);

  private:        /* variables */

  Network::Connection        *connection;
  Protocol::HandlerHash       handlers;
  Protocol::Handshaker       *handshaker;
  QTimer                      keepAliveTimer;
  QTimer                      pollTimer;
  bool                        registerAfterChallenge;
  Status                      state;
  VariableHash                variables;
};

}

#endif


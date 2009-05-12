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

#include <QHash>
#include <QTime>
#include <QTimer>

#include "common/actions.h"

#include "network/connection.h"

#include "protocol/aes.h"
#include "protocol/handshaker.h"
#include "protocol/handlers/include.list"
#include "protocol/enumerables/contact.h"
#include "protocol/enumerables/message.h"

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
  
  void captchaReceived(const QByteArray &);
  void errorEncountered(const QString &);
  void outgoingAction(Action);
  void outgoingMessage(const QString & contactAddress, const QString & message);
  void outgoingError(int code, const QString &message);
  void outgoingVariables(const VariableHash&);
  
  public slots:
  void addContact(const QString &group, const QString &contactAddress, const QString &nickname,
    Protocol::Enumerables::Contact::Type type, const QString &message);
    
  private slots:
  
  void incomingError(const QString &);
  void incomingPacket(const QByteArray &);
  void incomingVariables(const VariableHash &);
  void keepAlive();

  public:         /* methods */
  
  void authenticate(const VariableHash &settings);
  void allowSubscription(const QString &contactAddress, const QString &group,
    const QString &nickname);
  void denySubscription(const QString &contactAddress);
  void initialize();
  void login(const QString &cellphone, const QString &password, const QString &captcha);
  void removeContact(const QString &contactAddress);
  void setGateway(const QString &connectionString);
  void sendMessage(const QString &contactAddress, const QString &message,
    Protocol::Enumerables::Message::Type, unsigned int flags);
  
  QByteArray variableValue(const QString &name);

  private:        /* methods */
  
  MXit::Network::Packet* buildPacket();
  void challenge(const QString &cellphone, const QString &captcha);
  MXit::Network::Packet* getPacket(const QString &handler);
  MXit::Network::Packet* getPacket(const QString &handler, VariableHash &packetVariables);
  MXit::Protocol::Handler* handlerFor(const QByteArray &command);
  void initializationComplete();
  void sendPacket(const QString &handler);
  void sendPacket(const QString &handler, VariableHash &packetVariables);
  void setupReceived();
  void useVariable(const QString &variable, unsigned int index);

  private:        /* variables */
  
  MXit::Network::Connection  *connection;
  MXit::Protocol::HandlerHash handlers;
  MXit::Protocol::Handshaker *handshaker;
  QTimer                      keepAliveTimer;
  QTime                       lastAction;
  Status                      state;
  VariableHash                variables;
};

}

#endif


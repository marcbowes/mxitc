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

#include "network/connection.h"

#include "protocol/handshaker.h"
#include "protocol/handlers/include.list"

#include "aes.h"

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
  
  private slots:
  
  void incomingVariables(const VariableHash &);

  public:         /* methods */
  
  void initialize();
  void login(const QString &cellphone, const QString &password, const QString &captcha);

  private:        /* methods */
  
  MXit::Network::Packet* buildPacket();
  void challenge(const QString &cellphone, const QString &captcha);
  void initializationComplete();
  void setupReceived();
  void useVariable(const QString &variable, unsigned int index);

  private:        /* variables */
  
  MXit::Network::Connection  *connection;
  MXit::Protocol::Handshaker *handshaker;
  Status                      state;
  VariableHash                variables;
};

}

#endif


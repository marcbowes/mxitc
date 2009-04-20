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

#include "protocol/handshaker.h"

namespace MXit
{

typedef QHash<QString, QByteArray> VariableHash;

class Client : public QObject
{
  Q_OBJECT

  public:         /* class specific */
	
  Client();
  ~Client();

  signals:
  
  void captchaReceived(const QByteArray &);
  
  private slots:
  
  void incomingVariables(const VariableHash &);

  public:         /* methods */
  
  void initialize();
  void login(const QString &cellphone, const QString &password, const QString &captcha);

  private:        /* methods */
  
  void challenge(const QString &cellphone, const QString &captcha);

  private:        /* variables */
  
  MXit::Protocol::Handshaker *handshaker;
  VariableHash                variables;
};

}

#endif


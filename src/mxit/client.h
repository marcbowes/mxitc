/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** This is an extension of the generated dialog, made with Designer
**
****************************************************************************/

#ifndef __CLIENT_H__
#define __CLIENT_H__

#include<QObject>
#include "network/httpcomm.h"

namespace MXit
{

class Client : public QObject
{
  Q_OBJECT

  signals:
  void captchaReceived(const QByteArray &);
  
  private slots:
  void captchaReceivedClient(const QByteArray &);
  
  public:         /* class specific */
	
  Client(QObject *parent = 0);
  void getLoginInfo();
  void login(QString username, QString pass, QString captcha);
  
  
  private:
    
  MXit::HttpComm * httpComm;
};

}

#endif


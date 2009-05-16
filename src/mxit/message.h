/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::Message models a chat message (incoming or outgoing), for use
** by a Conversation.
**
****************************************************************************/

#ifndef __MXIT_MESSAGE_H__
#define __MXIT_MESSAGE_H__

#include <QString>
#include <QTime>

#include "contact.h"

namespace MXit
{

class Message
{
  public:         /* class specific */
  
  Message(const QString &message, const bool hasMarkup);
  Message(const Contact &contact, const QString &message, const QByteArray flags);
  Message(const Message &other);
  ~Message();
  
  static QString markup(const QString &markup);
  static bool flagDeliveryNotification(const QByteArray flags);
  static bool flagReadNotification(const QByteArray flags);
  static bool flagPasswordEncrypted(const QByteArray flags);
  static bool flagTransportEncrypted(const QByteArray flags);
  static bool flagReplyShouldBePasswordEncrypted(const QByteArray flags);
  static bool flagReplyShouldBeTransportEncrypted(const QByteArray flags);
  static bool flagContainsMarkup(const QByteArray flags);
  static bool flagContainsCustomEmoticons(const QByteArray flags);
  
  public:         /* variables */
  
  const Contact *contact;
  const QString  message;
  const QTime    timestamp;
  const bool deliveryNotification;
  const bool readNotification;
  const bool passwordEncrypted;
  const bool transportEncrypted;
  const bool replyShouldBePasswordEncrypted;
  const bool replyShouldBeTransportEncrypted;
  const bool containsMarkup;
  const bool containsCustomEmoticons;
};

typedef QList<Message*> MessageList;

}

#endif /* __MXIT_MESSAGE_H__ */


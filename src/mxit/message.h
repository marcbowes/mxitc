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
  
  /* flag creator methods */
  static bool flagContainsCustomEmoticons(const QByteArray flags);
  static bool flagContainsMarkup(const QByteArray flags);
  static bool flagDeliveryNotification(const QByteArray flags);
  static bool flagPasswordEncrypted(const QByteArray flags);
  static bool flagReadNotification(const QByteArray flags);
  static bool flagReplyShouldBePasswordEncrypted(const QByteArray flags);
  static bool flagReplyShouldBeTransportEncrypted(const QByteArray flags);
  static bool flagTransportEncrypted(const QByteArray flags);

  public:         /* methods */
  
  const QString message() const;
    
  public:         /* variables */
  
  const Contact *contact;
  const QString  markedupMessage;
  const QString  rawMessage;
  const QTime    timestamp;
  
  /* flags */
  const bool containsCustomEmoticons;
  const bool containsMarkup;
  const bool deliveryNotification;
  const bool passwordEncrypted;
  const bool readNotification;
  const bool replyShouldBePasswordEncrypted;
  const bool replyShouldBeTransportEncrypted;
  const bool transportEncrypted;
};

typedef QList<Message*> MessageList;

}

#endif /* __MXIT_MESSAGE_H__ */


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
  
  Message(const QString &message);
  Message(const Contact &contact, const QString &message);
  ~Message();

  public:         /* variables */
  
  const Contact *contact;
  const QString  message;
  const QTime    timestamp;
};

}

#endif /* __MXIT_MESSAGE_H__ */


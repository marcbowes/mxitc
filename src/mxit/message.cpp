/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QDebug>
#include <QTextDocument>

#include "message.h"
#include "protocol/enumerables/message.h"

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
** Constructor for outgoing chat (to a Contact)
**
****************************************************************************/
Message::Message(const QString &message, const bool hasMarkup)
  : contact(NULL), markedupMessage(markup(message)), rawMessage(message),
    timestamp(QTime::currentTime()),
    containsMarkup(hasMarkup),
    deliveryNotification(false),
    readNotification(false),
    passwordEncrypted(false),
    transportEncrypted(false),
    replyShouldBePasswordEncrypted(false),
    replyShouldBeTransportEncrypted(false),
    containsCustomEmoticons(false)
{
  /* Nothing */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Constructor for incoming chat (from a Contact)
**
****************************************************************************/
Message::Message(const Contact &contact, const QString &message, const QByteArray flags)
  : contact(&contact), markedupMessage(markup(message)), rawMessage(message),
    timestamp (QTime::currentTime()),
    deliveryNotification(flagDeliveryNotification(flags)),
    readNotification(flagReadNotification(flags)),
    passwordEncrypted(flagPasswordEncrypted(flags)),
    transportEncrypted(flagTransportEncrypted(flags)),
    replyShouldBePasswordEncrypted(flagReplyShouldBePasswordEncrypted(flags)),
    replyShouldBeTransportEncrypted(flagReplyShouldBeTransportEncrypted(flags)),
    containsMarkup(flagContainsMarkup(flags)),
    containsCustomEmoticons(flagContainsCustomEmoticons(flags))
{
  /* Nothing */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Copy constructor
**
****************************************************************************/
Message::Message(const Message &other)
  : contact(other.contact), 
    rawMessage(other.rawMessage),
    markedupMessage(other.markedupMessage),
    timestamp(other.timestamp),
    deliveryNotification(other.deliveryNotification),
    readNotification(other.readNotification),
    passwordEncrypted(other.passwordEncrypted),
    transportEncrypted(other.transportEncrypted),
    replyShouldBePasswordEncrypted(other.replyShouldBePasswordEncrypted),
    replyShouldBeTransportEncrypted(other.replyShouldBeTransportEncrypted),
    containsMarkup(other.containsMarkup),
    containsCustomEmoticons(other.containsCustomEmoticons)
{
  /* Nothing */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Default destructor
**
****************************************************************************/
Message::~Message()
{
  /* Nothing */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Implements MXit Markup
** All text is first CGI escaped, and then markup-up into HTML using MXit-
**  specifc rules.
**
****************************************************************************/
QString Message::markup(const QString &markup)
{
  /* CGI escape so that any HTML in the message isn't interpreted */
  QString escaped = Qt::escape(markup);
  QString markedUp;
  
  bool boldOpen = false;
  bool italicOpen = false;
  bool underlineOpen = false;
  
  int position = 0;
  
  while (position < escaped.length()) {
    if (escaped.at(position) != '\\') {
      if (escaped.at(position) == '/') {
        if (italicOpen) {
          markedUp += "</i>";
          italicOpen = false;
        } else {
          markedUp += "<i>";
          italicOpen = true;
        }
      } else if (escaped.at(position) == '*') {
        if (boldOpen) {
          markedUp += "</b>";
          boldOpen = false;
        } else {
          markedUp += "<b>";
          boldOpen = true;
        }
      } else if (escaped.at(position) == '_') {
        if (underlineOpen) {
          markedUp += "</u>";
          underlineOpen = false;
        } else {
          markedUp += "<u>";
          underlineOpen = true;
        }
      } else {
        markedUp += escaped.at(position);
      }
    } else {
      if ((position != (escaped.length() - 1)) && 
      ((escaped.at(position + 1) == '*') || (escaped.at(position+ 1) == '/') || (escaped.at(position + 1) == '_'))) {
        markedUp += escaped.at(position + 1);
        position++;
      } else {
        markedUp += escaped.at(position);
      }
    }
    
    position++;
  }
  
  if (boldOpen)
    markedUp += "</b>";
  if (italicOpen)
    markedUp += "</i>";
  if (underlineOpen)
    markedUp += "</u>";
  
  return markedUp;
}


/****************************************************************************
**
** Author: Tim Sjoberg
**
** Static flag methods..
**
****************************************************************************/

bool Message::flagDeliveryNotification(const QByteArray flags)
{
  return flags.toInt() & MXit::Protocol::Enumerables::Message::DeliveryNotification;
}


bool Message::flagReadNotification(const QByteArray flags)
{
  return flags.toInt() & MXit::Protocol::Enumerables::Message::ReadNotification;
}


bool Message::flagPasswordEncrypted(const QByteArray flags)
{
  return flags.toInt() & MXit::Protocol::Enumerables::Message::PasswordEncrypted;
}


bool Message::flagTransportEncrypted(const QByteArray flags)
{
  return flags.toInt() & MXit::Protocol::Enumerables::Message::TransportEncrypted;
}


bool Message::flagReplyShouldBePasswordEncrypted(const QByteArray flags)
{
  return flags.toInt() & MXit::Protocol::Enumerables::Message::ReplyPasswordEncrypted;
}


bool Message::flagReplyShouldBeTransportEncrypted(const QByteArray flags)
{
  return flags.toInt() & MXit::Protocol::Enumerables::Message::ReplyTransportEncrypted;
}


bool Message::flagContainsMarkup(const QByteArray flags)
{
  return flags.toInt() & MXit::Protocol::Enumerables::Message::MayContainMarkup;
}


bool Message::flagContainsCustomEmoticons(const QByteArray flags)
{
  return flags.toInt() & MXit::Protocol::Enumerables::Message::CustomEmoticons;
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
** Returns the display-ready version of the message's contact (markup or not).
**
****************************************************************************/
const QString Message::message() const
{
  return containsMarkup ? markedupMessage : rawMessage;
}

}


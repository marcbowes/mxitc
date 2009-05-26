/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QTextDocument>
#include <QRegExp>
#include <QStringList>

#include "protocol/enumerables/message.h"

#include "message.h"

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
Message::Message(const QString &message, const bool hasMarkup, const int messageType)
  : contact(NULL), markedupMessage(markup(message, NULL)), rawMessage(message),
    system(false),
    timestamp(QTime::currentTime()),

    containsMarkup(hasMarkup),
    type(messageType),
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
Message::Message(const Contact *contact, const QString &message, const QByteArray flags, const int messageType)
  : contact(contact), markedupMessage(markup(message, contact)), rawMessage(message),
    system(contact == NULL),
    timestamp (QTime::currentTime()),
    deliveryNotification(flagDeliveryNotification(flags)),
    readNotification(flagReadNotification(flags)),
    passwordEncrypted(flagPasswordEncrypted(flags)),
    transportEncrypted(flagTransportEncrypted(flags)),
    replyShouldBePasswordEncrypted(flagReplyShouldBePasswordEncrypted(flags)),
    replyShouldBeTransportEncrypted(flagReplyShouldBeTransportEncrypted(flags)),
    containsMarkup(flagContainsMarkup(flags)),
    containsCustomEmoticons(flagContainsCustomEmoticons(flags)),
    type(messageType)
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
    system(other.system),
    timestamp(other.timestamp),

    deliveryNotification(other.deliveryNotification),
    readNotification(other.readNotification),
    passwordEncrypted(other.passwordEncrypted),
    transportEncrypted(other.transportEncrypted),
    replyShouldBePasswordEncrypted(other.replyShouldBePasswordEncrypted),
    replyShouldBeTransportEncrypted(other.replyShouldBeTransportEncrypted),
    containsMarkup(other.containsMarkup),
    containsCustomEmoticons(other.containsCustomEmoticons),
    type(other.type)
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
** Author: Tim Sjoberg
**
** Implements MXit Commands (incomplete)
**
****************************************************************************/
QString Message::commandUp(const QString &markup, const Contact *contact)
{
  QStringList lines(markup.split('\n'));
  //FIXME: reply only hack
  QRegExp rx("::(.*):(.*)");
  QString markedUp;

  Q_FOREACH(const QString &line, lines) {
    if (!line.isEmpty()) {
      if (rx.indexIn(line) != -1) {
        markedUp += "<a href=\"mxit://";
        markedUp += contact->contactAddress;
        markedUp += "/";
        markedUp += rx.cap(1);
        markedUp += "/7";
        markedUp += "\">";
        markedUp += rx.cap(2);
        markedUp += "</a><br>";
      } else {
        markedUp += line;
        markedUp += "<br>";
      }
    }
  }

  return markedUp;
}


/****************************************************************************
**
** Author: Tim Sjoberg
** Author: Marc Bowes
**
** Implements MXit Markup
** All text is first CGI escaped, and then markup-up into HTML using MXit-
**  specifc rules.
**
****************************************************************************/
QString Message::markup(const QString &markup, const Contact *contact)
{
  /* HTML encode so that any HTML in the message isn't interpreted */
  QString escaped = Qt::escape(markup);

  /* setup */
  QRegExp finder("[\\*/_\\$]");                       /* find any of * / _ $ */
  bool    bold = 0, italic = 0, underline = 0;        /* tag counters so we can properly close */
  quint16 idx1 = 0, idx2   = 0;                       /* index of markup start and end */
  QString markedUp;                                   /* result */

  /* iteratively find markup candidates */
  while (idx1 < escaped.length()) {
    idx2 = escaped.indexOf(finder, idx1);

    /* found a tag */
    if (idx2 != quint16(-1)) {
      markedUp.append(escaped.mid(idx1, idx2 - idx1));     /* capture everything between the indices */

      if ((idx2 == 0 || idx2 == quint16(-1)) || escaped.at(idx2 - 1) != '\\') {
        /* append the correct HTML tag */
        switch (escaped.at(idx2).toAscii()) {
          case '*':
            if (bold)
              markedUp.append("</b>");
            else
              markedUp.append("<b>");
            bold = !bold;
            break;
          case '/':
            if (italic)
              markedUp.append("</i>");
            else
              markedUp.append("<i>");
            italic = !italic;
            break;
          case '_':
            if (underline)
              markedUp.append("</u>");
            else
              markedUp.append("<u>");
            underline = !underline;
            break;
          case '$':
            /* find next non-escaped $ */
            quint16 idx3 = 0;
            bool found = false;
            do {
              idx3 = escaped.indexOf('$', idx2 + 1);
              if (idx3 == quint16(-1))
                break;
              if (idx3 == 0 || escaped.at(idx3) != '\\')
                found = true;
            } while (!found);

            if (idx3 != quint16(-1)) {
              /* close all remaining tags, $ is a anti-markup barrier */
              if (bold) {
                markedUp.append("</b>");
                bold = false;
              }
              if (italic) {
                markedUp.append("</i>");
                italic = false;
              }
              if (underline) {
                markedUp.append("</u>");
                underline = false;
              }

              /* <a href="mxit://john/link/2">link</a>, where john is the contact and link is the text inside the $'s */
              QString ca = contact ? contact->contactAddress : "";
              QString ht = escaped.mid(idx2 + 1, idx3 - idx2 - 1);
              markedUp.append(QString("<a href=\"mxit://%1/%2/2\">%2</a>").arg(ca).arg(ht));

              idx2 = idx3;
            }

            break;
            /* no need for default */
        }
      }
      else {
        /* remove \ and append letter */
        markedUp.remove(markedUp.length() - 1, 1);
        markedUp.append(escaped.at(idx2));
      }

      idx1 = idx2 + 1;
    }
    /* no tag found, end loop */
    else {
      markedUp.append(escaped.mid(idx1));  /* append rest of string */

      /* close all remaining tags for valid HTML */
      if (bold)
        markedUp.append("</b>");
      if (italic)
        markedUp.append("</i>");
      if (underline)
        markedUp.append("</u>");

      idx1 = escaped.length();
    }
  }

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
  if (type == MXit::Protocol::Enumerables::Message::Command) {
    return commandUp(rawMessage, contact);
  } else
    return containsMarkup ? markedupMessage : rawMessage;
}

}


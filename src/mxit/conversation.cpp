/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QRegExp>
#include <QStringList>

#include "gui/emoticon.h" /* FIXME: reliance on GUI code */

#include "conversation.h"

namespace MXit
{

/****************************************************************************
        __                                _ ____    
   ____/ /__ ____ ___   ___ ___  ___ ____(_) _(_)___
  / __/ / _ `(_-<(_-<  (_-</ _ \/ -_) __/ / _/ / __/
  \__/_/\_,_/___/___/ /___/ .__/\__/\__/_/_//_/\__/ 
                         /_/                        

****************************************************************************/

const static QString initialHtml ("\
  <!DOCTYPE HTML \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\"> \
  <html> \
    <head> \
      <style></style> \
    </head> \
  <body> \
    <!-- start chat --> \
  </body> \
  </html> \
  ");

/****************************************************************************
**
** Author: Marc Bowes
**
** Constructor for a private Conversation
**
****************************************************************************/
Conversation::Conversation(const Contact *contact, const GUI::Theme &theme) /* FIXME: reliance on GUI code */
  : active(true), displayName(contact->nickname),
    uniqueIdentifier(contact->contactAddress), type(Protocol::Enumerables::Message::Normal),
    conversationHtml(initialHtml),
    theme(theme) /* FIXME: reliance on GUI code */
{
  contacts.insert(contact);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Constructor which accepts a pre-formed ContactSet
**
****************************************************************************/
Conversation::Conversation(const ContactSet &contacts, const GUI::Theme &theme, const QString &roomName)
  : active(true), displayName(roomName.isEmpty() ? buildDisplayName(contacts) : roomName),
    uniqueIdentifier(roomName), type(Protocol::Enumerables::Message::GroupChat),
    conversationHtml(initialHtml),
    theme(theme) /* FIXME: reliance on GUI code */
{
  this->contacts = contacts; /* copy */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Default destructor
**
****************************************************************************/
Conversation::~Conversation()
{
  Q_FOREACH(Message *message, messages)
    delete message;
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
** Inserts a Contact into the internal ContactSet.
** This method is safe - if the Contact is already stored, nothing will
**  happen.
**
****************************************************************************/
void Conversation::addContact(const Contact *contact)
{
  contacts.insert(contact);
  emit updated(this);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Units a ContactList against the internal ContactSet.
** This method is safe - the list can contain Contacts already stored and
**  it will simply skip them.
**
****************************************************************************/
void Conversation::addContacts(const ContactList &contacts)
{
  this->contacts.unite(ContactSet::fromList(contacts));
  emit updated(this);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Stores a Message in the Conversation
**
****************************************************************************/
void Conversation::appendMessage(const Message &message)
{
  QString cssClass  = message.contact ? "" : (message.system ? " system" : " you");
  QString author    = message.contact ? message.contact->nickname : (message.system ? "System" : "You");
  
  /* build insertion */
  QString insertion;
  insertion.append(QString("<div class=\"message%1\">").arg(cssClass));
  insertion.append(QString("<span class=\"timestamp\">[%1]</span>").arg(message.timestamp.toString()));
  insertion.append(QString("<span class=\"author\">%1</span>").arg(author));
  QString messageWithEmoticons = message.message(); /* FIXME: reliance on GUI code */
  theme.chat.injectEmoticons(messageWithEmoticons);
  insertion.append(QString("<span class=\"body\">%1</span>").arg(messageWithEmoticons));
  insertion.append(        "<br class=\"clear\" />");
  insertion.append(        "</div>");
  conversationHtml.insert(conversationHtml.size() - 21, insertion);
  messages.append(new Message(message));
  emit messageAdded(this);
  emit updated(this);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Returns the ContactSet in this Conversation
**
****************************************************************************/
const ContactSet& Conversation::getContacts() const
{
  return contacts;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Returns a timestamp representing when last this Conversation was updated.
**
****************************************************************************/
QTime Conversation::lastTimestamp() const
{
  if (messages.isEmpty())
    return QTime();
  else
    return messages.last()->timestamp;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Removes a Contact from the interal ContactSet.
** This method works regardless of whether the Contact was in this
**  Conversation.
**
****************************************************************************/
void Conversation::removeContact(const Contact *contact)
{
  contacts.remove(contact);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Rewrites HEAD to use the location specified.
** Note: don't worry about QString#replace being global, because chat is
**  HTML-escaped, so only concern is performance..
**
****************************************************************************/
void Conversation::setCss(const QString &css)
{
  QRegExp rx("<style>(.*)</style>");
  conversationHtml.replace(rx, QString("<style>%1</style>").arg(css));
  /* rip out all emoticon img tags */
  GUI::Emoticon::HtmlToShorthand(conversationHtml); /* FIXME: reliance on GUI code */
  /* inject new img tags from new theme (if any) */
  theme.chat.injectEmoticons(conversationHtml); /* FIXME: reliance on GUI code */
  emit updated(this);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Toggles the active state of this Conversation.
**
****************************************************************************/
void Conversation::toggleActive()
{
  active = !active;
  emit updated(this);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Removes a Contact for each Contact in the given ContactList
**
****************************************************************************/
void Conversation::removeContacts(const ContactList &contacts)
{
  Q_FOREACH(const Contact *contact, contacts)
    removeContact(contact);
}


/****************************************************************************
               _           __                  __  __           __  
     ___  ____(_)  _____ _/ /____   __ _  ___ / /_/ /  ___  ___/ /__
    / _ \/ __/ / |/ / _ `/ __/ -_) /  ' \/ -_) __/ _ \/ _ \/ _  (_-<
   / .__/_/ /_/|___/\_,_/\__/\__/ /_/_/_/\__/\__/_//_/\___/\_,_/___/
  /_/                                                               

****************************************************************************/


/****************************************************************************
**
** Author: Marc Bowes
**
** Creates a displayName from the ContactSet's contactAddress's.
**
****************************************************************************/
QString Conversation::buildDisplayName(const ContactSet &contacts)
{
  QStringList list;
  Q_FOREACH(const Contact *contact, contacts)
    list << contact->nickname;
  return list.join(", ");
}

}


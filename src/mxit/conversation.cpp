/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QRegExp>
#include <QStringList>

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
      <link href=\"stylesheet.css\" rel=\"stylesheet\" type=\"text/css\" /> \
    </head> \
  <body> \
    <table></table> \
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
Conversation::Conversation(const Contact *contact)
  : active(true), displayName(contact->nickname),
    uniqueIdentifier(contact->contactAddress), type(Private), conversationHtml(initialHtml)
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
Conversation::Conversation(const ContactSet &contacts, const QString &roomName)
  : active(true), displayName(roomName.isEmpty() ? buildDisplayName(contacts) : roomName),
    uniqueIdentifier(roomName), type(Group), conversationHtml(initialHtml)
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
  QString thclass = message.contact ? "" : " class=\"you\"";
  QString author  = message.contact ? message.contact->nickname : "You";
  QString insertion = QString("<tr><th%1>%2</th><td>%3</td></tr>")
    .arg(thclass).arg(author)
    .arg(message.message);
  conversationHtml.insert(conversationHtml.size()- 31, insertion);
  messages.append(new Message(message));
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
void Conversation::setCss(const QString &location)
{
  QRegExp rx("<link href=\"(.*)\" rel=\"stylesheet\" type=\"text/css\" />");
  conversationHtml.replace(rx,
    QString("<link href=\"file://%1/chat/stylesheet.css\" rel=\"stylesheet\" type=\"text/css\" />").arg(location));
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


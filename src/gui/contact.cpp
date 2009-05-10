/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QDebug>

#include "contact.h"

namespace MXit
{

namespace GUI
{

/****************************************************************************
**
** Author: Richard Baxter
**
** Contact constructor
**
****************************************************************************/

Contact:: Contact(
      const QString& group, 
      const QString& contactAddress, 
      const QString& nickname, 
      unsigned int presence, 
      unsigned int type, 
      unsigned int mood) :
      group(group), contactAddress(contactAddress), nickname(nickname), presence((Presence)presence), type((Type)type), mood((Mood)mood), unreadMessage(false)
{
  /* nothing */
}


/****************************************************************************
**
** Author: Richard Baxter
**
** Contact copy constructor
**
****************************************************************************/

Contact:: Contact(const Contact& c) :
      group(c.group), contactAddress(c.contactAddress), nickname(c.nickname), presence(c.presence), type(c.type), mood(c.mood), unreadMessage(c.unreadMessage)
{
  /* nothing */
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Contact deconstructor
**
****************************************************************************/
Contact::~Contact()
{
  /* nothing */
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

Contact& Contact::operator=(const Contact& c) {
  group = c.group;
  contactAddress = c.contactAddress;
  nickname = c.nickname;
  presence = c.presence;
  type = c.type;
  mood = c.mood;
  unreadMessage = c.unreadMessage;
  
  return *this;
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contact::incomingMessage(Message message)
{
  chatHistoryVec.append(message);
  unreadMessage = true;
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

const QVector <Message>& Contact::chatHistory()
{
  return chatHistoryVec;
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

QString Contact::getFormattedMsg(int i) const /* FIXME deprecated i think*/
{
  const Message& m = chatHistoryVec[i];
  return (m.sender()?m.sender()->getNickname():QString("You")) + ": " + m.message();
}

  
/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

QString Contact::getNickname() const
{
  return nickname;
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

QString Contact::getContactAddress() const
{
  return contactAddress;
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

QString Contact::getGroup() const
{
  return group;
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contact::setGroup(const QString& group)
{
  this->group = group;
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contact::setContactAddress(const QString& contactAddress)
{
  this->contactAddress = contactAddress;
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contact::setNickname(const QString& nickname)
{
  this->nickname = nickname;
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contact::setPresence(int presence)
{
  this->presence = (Presence)presence;
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contact::setType(int type)
{
  this->type = (Type)type;
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contact::setMood(int mood)
{
  this->mood = (Mood)mood;
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void  Contact::setChatInputText(const QString& chatInputText_) {
  chatInputText = chatInputText_;
}
  
  
/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
QString  Contact::getChatInputText() {
  return chatInputText;
}


} /* end namespace GUI */

} /* end namespace MXit */



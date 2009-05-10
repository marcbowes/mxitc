/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "contact.h"

namespace MXit
{

namespace GUI
{

/****************************************************************************
**
** Author: Marc Bowes
**
** Contact constructor
**
****************************************************************************/
Contact::Contact()
{
  /* nothing */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Contact copy constructor
**
****************************************************************************/
Contact::Contact(const Contact &other)
{
  this->group           = other.group;
  this->contactAddress  = other.contactAddress;
  this->nickname        = other.nickname;
  this->presence        = other.presence;
  this->type            = other.type;
  this->mood            = other.mood;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Contact deconstructor
**
****************************************************************************/
Contact::~Contact()
{
  /* nothing */
}


Contact& Contact::operator=(const Contact &other)
{
  group                 = other.group;
  contactAddress        = other.contactAddress;
  nickname              = other.nickname;
  presence              = other.presence;
  type                  = other.type;
  mood                  = other.mood;
  unreadMessage         = true;

  return *this;
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void Contact::incomingMessage(Message message)
{
  chatHistory.append(message);
  unreadMessage = true;
}

}

}

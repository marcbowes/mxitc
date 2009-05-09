/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "contact.h"

namespace MXit
{

/****************************************************************************
**
** Author: Marc Bowes
**
** Contact constructor
**
****************************************************************************/
Contact::Contact(const QString &group, const QString &contactAddress, const QString &nickname,
    quint16 presence, quint16 type, quint16 mood)
{
  this->group           = group;
  this->contactAddress  = contactAddress;
  this->nickname        = nickname;
  this->presence        = (Protocol::Enumerables::Presence)presence;
  this->type            = (Protocol::Enumerables::Contact)type;
  this->mood            = (Protocol::Enumerables::Mood)mood;  
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

}


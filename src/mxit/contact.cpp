/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "contact.h"

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
** Contact default constructor
**
****************************************************************************/
Contact::Contact()
{
  this->presence        = Protocol::Enumerables::Contact::Offline;
  this->type            = Protocol::Enumerables::Contact::MXit;
  this->mood            = Protocol::Enumerables::Contact::None;
}


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
  this->presence        = (Protocol::Enumerables::Contact::Presence)presence;
  this->type            = (Protocol::Enumerables::Contact::Type)type;
  this->mood            = (Protocol::Enumerables::Contact::Mood)mood;
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


/****************************************************************************
**
** Author: Marc Bowes
**
** Equality operator
**
****************************************************************************/
bool Contact::operator==(const Contact &other)
{
  return this->contactAddress == other.contactAddress;
}

}


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
** Updates fields (except contactAddress) from raw MXit data
**
****************************************************************************/
void Contact::updateFromRaw(const QList<QByteArray> &fields)
{
  group     = fields[0];
  /* skip contactAddress */
  nickname  = fields[2];
  presence  = (Protocol::Enumerables::Contact::Presence)fields[3].toUInt();
  type      = (Protocol::Enumerables::Contact::Type)fields[4].toUInt();
  mood      = (Protocol::Enumerables::Contact::Mood)fields[5].toUInt();
}

}


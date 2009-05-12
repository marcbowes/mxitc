/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** Provides functionality to store contacts as an object
**
****************************************************************************/

#ifndef __MXIT_CONTACT_H__
#define __MXIT_CONTACT_H__

#include <QString>

#include "protocol/enumerables/contact.h"

namespace MXit
{

class Contact
{
  public:         /* class specific */
	
	Contact();
  Contact(const QString &group, const QString &contactAddress, const QString &nickname,
    quint16 presence, quint16 type, quint16 mood);
  ~Contact();
  
  public:         /* variables */

  Protocol::Enumerables::Contact::Presence presence;
  Protocol::Enumerables::Contact::Type     type;
  Protocol::Enumerables::Contact::Mood     mood;

  public:        /* variables */
  
  QString contactAddress;
  QString group;
  QString inviteMessage;
  bool    hideAddress;
  QString nickname;
};

}

#endif


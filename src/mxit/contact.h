/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** Model for a MXit contact (address, presence..)
**
****************************************************************************/

#ifndef __MXIT_CONTACT_H__
#define __MXIT_CONTACT_H__

#include <QHash>
#include <QList>
#include <QSet>
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
  
  bool operator==(const Contact &other);
  
  public:        /* methods */
  
  QString sortString();
  void updateFromRaw(const QList<QByteArray> &fields);
  
  public:        /* variables */

  Protocol::Enumerables::Contact::Presence presence;
  Protocol::Enumerables::Contact::Type     type;
  Protocol::Enumerables::Contact::Mood     mood;

  public:       /* variables */
  
  QString contactAddress;
  QString group;
  QString inviteMessage;
  bool    hideAddress;
  QString nickname;
};

typedef QHash<QString, Contact*> ContactHash;
typedef QList<const Contact*>   ContactList;
typedef QSet<const Contact*>    ContactSet;

}

#endif


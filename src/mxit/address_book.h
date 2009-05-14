/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::AddressBook is essentially a list of Contacts, and provides methods
** to manage (add, remove and update) this list
**
****************************************************************************/

#ifndef __MXIT_ADDRESSBOOK_H__
#define __MXIT_ADDRESSBOOK_H__

#include <QByteArray>
#include <QList>
#include <QMap>
#include <QString>

#include "contact.h"

namespace MXit
{

typedef QMap <QString, Contact*> OrderedContactMap;

class AddressBook
{
  public:         /* class specific */
  
  AddressBook();
  ~AddressBook();
  
  public:         /* methods */

  void addContact(const QByteArray &data);
  void addContact(const QList<QByteArray> &fields);
  void addContacts(const QByteArray &data);
  Contact* contactFromAddress(const QString &contactAddress);
  const OrderedContactMap& getContacts();
  void removeContact(const QString &contactAddress);
  void updateContact(const QByteArray &data);
  void updateContact(const QList<QByteArray> &fields);
  void updateContacts(const QByteArray &data);
  
  private:       /* methods */
  
  void insertContact(const QList<QByteArray> &fields);
  
  private:       /* variables */
  
  ContactHash       contacts;
  OrderedContactMap ordered;
};

}

#endif /* __MXIT_ADDRESSBOOK_H__ */


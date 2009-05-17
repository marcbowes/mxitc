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
#include <QDebug>
#include <QByteArray>
#include <QList>
#include <QMap>
#include <QString>

#include "contact.h"

namespace MXit
{

typedef QMap<QString, Contact*> OrderedContactMap;

class AddressBook : public QObject
{
  Q_OBJECT
  
  public:         /* class specific */
  
  AddressBook();
  ~AddressBook();
  
  signals:
  
  void presenceToggled(const Contact*);
  void updated(const ContactList&);
  
  public:         /* methods */

  ContactList addOrUpdateContacts(const QByteArray &data);
  ContactList addSubscriptions(const QByteArray &data);
  Contact* contactFromAddress(const QString &contactAddress);
  const OrderedContactMap& getContacts();
  void removeContact(const QString &contactAddress);
  void updateContact(const QString &group, const QString &contactAddress,
    const QString &nickname);
  
  private:       /* methods */
  
  Contact* addOrUpdateContact(const QByteArray &data);
  Contact* addOrUpdateContact(const QList<QByteArray> &fields);
  Contact* addSubscription(const QByteArray &data);
  Contact* addSubscription(const QList<QByteArray> &fields);
  Contact* insertContact(const QList<QByteArray> &fields);
  Contact* insertSubscription(const QList<QByteArray> &fields);
  Contact* updateContact(const QList<QByteArray> &fields);
  
  private:       /* variables */
  
  ContactHash       contacts;
  QHash<QString, QString>
                    orderLookup;
  OrderedContactMap ordered;
};

}

#endif /* __MXIT_ADDRESSBOOK_H__ */


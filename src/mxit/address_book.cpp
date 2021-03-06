/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "address_book.h"

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
** Default constructor
**
****************************************************************************/
AddressBook::AddressBook()
{
  /* STUB */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Default destructor
**
****************************************************************************/
AddressBook::~AddressBook()
{
  Q_FOREACH(const Contact *contact, contacts)
    delete contact;
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
** Invokes #addContact for each Contact found in the MXit data.
**
****************************************************************************/
ContactList AddressBook::addOrUpdateContacts(const QByteArray &data)
{
  ContactList changeSet;
  Q_FOREACH(const QByteArray &row, data.split('\0')) {
    Contact *changed = addOrUpdateContact(row);
    if (changed)
      changeSet.append(changed);
  }
  
  if (!changeSet.isEmpty())
    emit updated(changeSet);
  
  return changeSet;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Adds new subscriptions to the AddressBook.
**
****************************************************************************/
ContactList AddressBook::addSubscriptions(const QByteArray &data)
{
  ContactList changeSet;
  Q_FOREACH(const QByteArray &row, data.split('\0')) {
    Contact *contact = addSubscription(row);
    if (contact)
      changeSet.append(contact);
  }
  
  if (!changeSet.isEmpty())
    emit updated(changeSet);
  
  return changeSet;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Returns a Contact* from a given address, or NULL if non-existant.
**
****************************************************************************/
Contact* AddressBook::contactFromAddress(const QString &contactAddress)
{
  return contacts.value(contactAddress);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Returns a [presence, name]-ordered list of Contacts
**
****************************************************************************/
const OrderedContactMap& AddressBook::getContacts()
{
  return ordered;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Permanently removes a Contact from the AddressBook.
** This method is safe - removing a non-existant Contact will have no effect.
**
****************************************************************************/
void AddressBook::removeContact(const QString &contactAddress)
{
  if (contacts.contains(contactAddress)) {
    Contact *_delete = *contacts.find(contactAddress);
    contacts.remove(_delete->contactAddress); /* from store */
    ordered.remove(orderLookup[_delete->contactAddress]); /* from order */
    orderLookup.remove(_delete->contactAddress); /* from lookup */
    delete _delete;
  }
}


/****************************************************************************
**
** Author: Marc Bowes
**
** For packet #5
**
****************************************************************************/
void AddressBook::updateContact(const QString &group, const QString &contactAddress,
  const QString &nickname)
{
  Contact *contact = contactFromAddress(contactAddress);
  if (contact) {
    contact->group = group;
    contact->nickname = nickname;
    
    /* hacky means of emitting an update */
    ContactList hack;
    hack.append(contact);
    emit updated(hack);
  }
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
** Splits data into fields and forwards to real method.
**
****************************************************************************/
Contact* AddressBook::addOrUpdateContact(const QByteArray &data)
{
  if (data.isEmpty())
    return NULL;
  
  /* for data format, refer to Handler #03, GetContacts */
  QList<QByteArray> fields = data.split('\1');
  
  return addOrUpdateContact(fields);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Builds a Contact from MXit data and updates the internal list.
** This method is safe - adding an existing Contact will auto-forward to
**  #updateContact
**
****************************************************************************/
Contact* AddressBook::addOrUpdateContact(const QList<QByteArray> &fields)
{
  if (contacts.contains(fields[1]))
    return updateContact(fields);
  else
    return insertContact(fields);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Splits data into fields and forwards to real method.
**
****************************************************************************/
Contact* AddressBook::addSubscription(const QByteArray &data)
{
  if (data.isEmpty())
    return NULL;
  
  /* for data format, refer to Handler #51, GetNewSubscription */
  QList<QByteArray> fields = data.split('\1');
  
  return addSubscription(fields);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Builds a Contact from MXit data and updates the internal list.
**
****************************************************************************/
Contact* AddressBook::addSubscription(const QList<QByteArray> &fields)
{
  if (!contacts.contains(fields[0]))
    return insertSubscription(fields);
  else
    return NULL;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Inserts a Contact into the local Contact store. Also inserts the Contact
**  into an ordered structure.
** **REQUIRES** the contact to not exist.
**
****************************************************************************/
Contact* AddressBook::insertContact(const QList<QByteArray> &fields)
{  
  /* type conversions (really only for neatness) */
  QString group           = fields[0];
  QString contactAddress  = fields[1];
  QString nickname        = fields[2];
  quint16 presence        = fields[3].toUInt();
  quint16 type            = fields[4].toUInt();
  quint16 mood            = fields[5].toUInt();
  
  /* insert into storage */
  Contact *contact = new Contact(group, contactAddress, nickname,
    presence, type, mood);
  contacts.insert(contactAddress, contact);
  
  /* ordered insert (QMap uses heaps to order keys) */
  orderLookup.insert(contact->contactAddress, contact->sortString());
  ordered.insert(contact->sortString(), contact);
  
  return contact;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Inserts a Contact into the local Contact store. Also inserts the Contact
**  into an ordered structure.
** **REQUIRES** the contact to not exist.
**
****************************************************************************/
Contact* AddressBook::insertSubscription(const QList<QByteArray> &fields)
{
  /* type conversions (really only for neatness) */
  QString contactAddress  = fields[0];
  QString nickname        = fields[1];
  quint16 type            = fields[2].toUInt();
  bool    hiddenLogin     = fields[3] == "1" ? true : false;
  QString joinMessage     = fields[4];
  
  /* insert into storage */
  Contact *contact = new Contact(contactAddress, nickname, type, hiddenLogin, joinMessage);
  contacts.insert(contactAddress, contact);
  
  /* ordered insert (QMap uses heaps to order keys) */
  orderLookup.insert(contact->contactAddress, contact->sortString());
  ordered.insert(contact->sortString(), contact);
  
  return contact;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Updates a Contact with new fields, returning a Contact* if the Presence
**  changed (i.e. reordered).
** **REQUIRES** the contact to exist.
**
****************************************************************************/
Contact* AddressBook::updateContact(const QList<QByteArray> &fields)
{
  /* find the Contact */
  OrderedContactMap::iterator itr = ordered.find(orderLookup[fields[1]]);
  Contact *contact = itr.value(); /* will break for itr == contacts.end() */
  
  /* remember Presence - this is important for ordering! */
  Protocol::Enumerables::Contact::Presence current = contact->presence;
  
  /* perform update */
  contact->updateFromRaw(fields);
  
  /* now reorder (reinsert) if Presence has changed */
  if (current != contact->presence) {
    ordered.erase(itr);
    orderLookup[contact->contactAddress] = contact->sortString();
    ordered.insert(contact->sortString(), contact);
    emit presenceToggled(contact);
    return contact;
  }
  
  /* not changed */
  return NULL;
}

}


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
** Splits data into fields and forwards to real method
**
****************************************************************************/
void AddressBook::addContact(const QByteArray &data)
{
  /* for data format, refer to Handler #03, GetContacts */
  QList<QByteArray> fields = data.split('\1');
  
  addContact(fields);
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
void AddressBook::addContact(const QList<QByteArray> &fields)
{
  if (contacts.contains(fields[0]))
    updateContact(fields);
  else
    insertContact(fields);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Invokes #addContact for each Contact found in the MXit data
**
****************************************************************************/
void AddressBook::addContacts(const QByteArray &data)
{
  Q_FOREACH(const QByteArray &row, data.split('\0'))
    addContact(row);
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
    contacts.remove(_delete->contactAddress);
    delete _delete;
  }
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Splits data into fields and forwards to real method
**
****************************************************************************/
void AddressBook::updateContact(const QByteArray &data)
{
  /* for data format, refer to Handler #03, GetContacts */
  QList<QByteArray> fields = data.split('\1');
  
  updateContact(fields);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Updates a Contact from MXit data.
** This method is safe - adding an non-existant Contact will auto-forward to
**  #insertContact
**
****************************************************************************/
void AddressBook::updateContact(const QList<QByteArray> &fields)
{
  if (!contacts.contains(fields[0]))
    insertContact(fields);
  else
    (*contacts.find(fields[0]))->updateFromRaw(fields);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Invokes #updateContact for each Contact found in the MXit data
**
****************************************************************************/
void AddressBook::updateContacts(const QByteArray &data)
{
  Q_FOREACH(const QByteArray &row, data.split('\0'))
    updateContact(row);
}


/****************************************************************************
               _           __                  __  __           __  
     ___  ____(_)  _____ _/ /____   __ _  ___ / /_/ /  ___  ___/ /__
    / _ \/ __/ / |/ / _ `/ __/ -_) /  ' \/ -_) __/ _ \/ _ \/ _  (_-<
   / .__/_/ /_/|___/\_,_/\__/\__/ /_/_/_/\__/\__/_//_/\___/\_,_/___/
  /_/                                                               

****************************************************************************/


void AddressBook::insertContact(const QList<QByteArray> &fields)
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
  ordered.insert(contact->sortString(), contact);
}

}


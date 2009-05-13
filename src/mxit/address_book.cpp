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
** Builds a Contact from MXit data and updates the internal list.
** This method is safe - adding an existing Contact will auto-forward to
**  #updateContact
**
****************************************************************************/
void AddressBook::addContact(const QByteArray &data)
{
  /* for data format, refer to Handler #03, GetContacts */
  
  QList<QByteArray> fields = data.split('\1');
  contacts.insert(new Contact(fields[0], fields[1], fields[2],
    Protocol::Enumerables::Contact::Presence(fields[3].toInt()),
    Protocol::Enumerables::Contact::Type(fields[4].toInt()),
    Protocol::Enumerables::Contact::Mood(fields[5].toInt())));
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
** Permanently removes a Contact from the AddressBook
**
****************************************************************************/
void AddressBook::removeContact(const QString &contactAddress)
{
  /* STUB */
}

}


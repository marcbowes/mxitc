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

#include <QString>

#include "contact.h"

namespace MXit
{

class AddressBook
{
  public:         /* class specific */
  
  AddressBook();
  ~AddressBook();
  
  public:         /* methods */

  void addContact(const QByteArray &data);
  void addContacts(const QByteArray &data);
  void removeContact(const QString &contactAddress);
  
  private:       /* variables */
  
  ContactSet      contacts;
};

}

#endif /* __MXIT_ADDRESSBOOK_H__ */


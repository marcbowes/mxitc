/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "conversation.h"

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
Conversation::Conversation()
{
  /* Nothing */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Constructor which accepts a pre-formed ContactSet
**
****************************************************************************/
Conversation::Conversation(const ContactSet &contacts)
{
  this->contacts = contacts; /* copy */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Default destructor
**
****************************************************************************/
Conversation::~Conversation()
{
  /* STUB */
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
** Inserts a Contact into the internal ContactSet.
** This method is safe - if the Contact is already stored, nothing will
**  happen.
**
****************************************************************************/
void Conversation::addContact(const Contact *contact)
{
  contacts.insert(contact);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Units a ContactList against the internal ContactSet.
** This method is safe - the list can contain Contacts already stored and
**  it will simply skip them.
**
****************************************************************************/
void Conversation::addContacts(const ContactList &contacts)
{
  this->contacts.unite(ContactSet::fromList(contacts));
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Removes a Contact from the interal ContactSet.
** This method works regardless of whether the Contact was in this
**  Conversation.
**
****************************************************************************/
void Conversation::removeContact(const Contact *contact)
{
  contacts.remove(contact);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Removes a Contact for each Contact in the given ContactList
**
****************************************************************************/
void Conversation::removeContacts(const ContactList &contacts)
{
  Q_FOREACH(const Contact *contact, contacts)
    removeContact(contact);
}

}


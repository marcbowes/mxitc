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
  /* STUB */
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
**
** Author: Marc Bowes
**
** Units a set of contacts against the internal ContactSet.
** This method is safe - the list can contain contacts already stored and
**  it will simply update any information which has changed.
**
****************************************************************************/
void Conversation::addOrUpdateContacts(const QList<Contact*> &contacts)
{
  this->contacts.unite(ContactSet::fromList(contacts));
}

}


/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
** Author: Richard Baxter, 2009
**
** GUI flavour of a contact (extends common/contact)
** provides functionality to display & chat with a contact
**
****************************************************************************/

#ifndef __MXIT_GUI_CONTACT_H__
#define __MXIT_GUI_CONTACT_H__

#include "common/contact.h"

#include "message.h"

namespace MXit
{

namespace GUI
{

class Contact: public MXit::Contact
{
  public:         /* class specific */
  
  Contact();
  Contact(const Contact &other);
  ~Contact();
  
  public:         /* methods */
  
  Contact& operator=(const Contact &other);
  
  void incomingMessage(Message message);
  
  public:         /* variables */
  
  MessageVec  chatHistory;
  QString     chatInputText;
  bool        unreadMessage;
};

}

}

#endif /* __MXIT_GUI_CONTACT_H__ */


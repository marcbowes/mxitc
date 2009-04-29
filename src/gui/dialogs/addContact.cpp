/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "addContact.h"

namespace MXit
{

namespace GUI

{

namespace Dialog

{

/****************************************************************************
**
** Author: Richard Baxter
**
** Dialog constructor
**
** ==== Arguments
** - app: owned by main.cpp
** - client: owned by main.cpp
**
****************************************************************************/
AddContact::AddContact(QWidget *parent, MXit::Client *client) : QDialog (parent)
{
  setupUi(this);      /* from ui_dialog.h: generated from dialog.ui */
  mxit = client;      /* store a copy */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Dialog deconstructor
**
****************************************************************************/
AddContact::~AddContact()
{
  // nothing here
}



} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */

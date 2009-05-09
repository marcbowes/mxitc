

#include "contacts.h"

namespace MXit
{

namespace GUI
{

namespace DockWidget
{



/****************************************************************************
**
** Author: Richard Baxter
**
** Widget constructor
**
****************************************************************************/
Contacts::Contacts(QWidget* parent) : MXitDockWidget(parent)
{
  setupUi(this);
  
  connect(contactList, SIGNAL(itemPressed ( QListWidgetItem *  )), this, SIGNAL(outgoingItemPressed( QListWidgetItem *  )));
  
}



/****************************************************************************
**
** Author: Richard Baxter
**
** Widget destructor
**
****************************************************************************/
Contacts::~Contacts()
{

}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contacts::clearList(){
  contactList->clear();
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contacts::addItemToList(QString& nickname){
  contactList->addItem(nickname);
}

} /* end of DockWidget namespace */


} /* end of GUI namespace */

} /* end of MXit namespace */





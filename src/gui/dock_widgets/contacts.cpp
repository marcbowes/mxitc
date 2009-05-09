

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
  clearList();
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contacts::clearList(){

  while(contactList->count()) {
    delete contactList->takeItem (0);
  }
  
  contactList->clear();
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contacts::addItemToList(const QListWidgetItem & liw){
  QListWidgetItem * item = new QListWidgetItem(liw);
  contactList->addItem(item);
}



} /* end of DockWidget namespace */


} /* end of GUI namespace */

} /* end of MXit namespace */





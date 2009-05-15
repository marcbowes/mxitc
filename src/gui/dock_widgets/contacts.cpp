/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QSet>
#include <QMenu>
#include <QMessageBox>

#include "contacts.h"
#include "gui/dialogs/allow_subscription.h"

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
Contacts::Contacts(QWidget* parent, Theme &theme, MXit::Client& mxit, AddressBook & addressBook) : MXitDockWidget(parent, theme), mxit(mxit), addressBook(addressBook)
{
  setupUi(this);
  
  /*connect(  contactsList, 
            SIGNAL(itemPressed ( QListWidgetItem *  )), 
            this, 
            SIGNAL(outgoingItemPressed( QListWidgetItem *  )));*///depricated
  
  
  connect(
        contactsList, 
        SIGNAL( customContextMenuRequested ( const QPoint & ) ), 
        this, 
        SLOT( popUpContextMenu( const QPoint & ) )  );
        
        
  connect(
        contactsList, 
        SIGNAL( itemDoubleClicked ( QListWidgetItem * ) ),
        this, 
        SIGNAL( chatRequest( QListWidgetItem * ) )  );
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
                __   ___           __     __    
     ___  __ __/ /  / (_)___  ___ / /__  / /____
    / _ \/ // / _ \/ / / __/ (_-</ / _ \/ __(_-<
   / .__/\_,_/_.__/_/_/\__/ /___/_/\___/\__/___/
  /_/                                           

****************************************************************************/


/****************************************************************************
**
** Author: Richard Baxter
**
** emits a signal containing the contact selected
**
****************************************************************************/

void Contacts::refreshThemeing() {

  /*refreshing all contacts*/
  for (int i = 0 ; i < contactsList->count() ; i++) {
    QListWidgetItem * lwi = contactsList->item(i);
    
    refreshListWidgetItem(lwi);
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
** Author: Richard Baxter
**
** refreshes a listWidgetItem (just icon for now)
**
****************************************************************************/

void Contacts::refreshListWidgetItem(QListWidgetItem *item) {
  Contact * contact = lwiToContact[item];
  item->setIcon(QPixmap(16,16)/*theme.contact.presence.pixmap(conversation->presence)*/);
  item->setText(contact->nickname);
}


/****************************************************************************
**
** Author: Richard Baxter
**
** pops up a context menu
**
****************************************************************************/

#define MENU_START(title) QMenu contextMenu(title, this); QHash<QString, QAction*> hash;
#define MENU_ITEM(y) hash[y] = new QAction (y, this); contextMenu.addAction(hash[y]);
#define MENU_EXEC(s) \
{ \
QAction* a = contextMenu.exec( pos ); \
bool __ret = true; \
if (a){ s = a->text(); __ret = false;} \
Q_FOREACH(QAction * act, hash){ delete act; } \
if (__ret) return;  \
}

void Contacts::popUpContextMenu(const QPoint & lovalPos) {
  
  Contact * contact;
  QPoint pos;
  
  QListWidgetItem * lwi = (contactsList->itemAt ( pos.x(), pos.y() ));
  if (lwi) {
    pos = contactsList->mapToGlobal ( pos );
    contact = lwiToContact[lwi];
  }
  else
    return;
  
  MENU_START(contact->nickname);
  
  if (contact->presence == Protocol::Enumerables::Contact::Unaffiliated) {
    MENU_ITEM("Accept");
    MENU_ITEM("Reject");
    MENU_ITEM("Reject Permanently");
  }
  else {
    MENU_ITEM("Chat");
    MENU_ITEM("Change Nickname");
    MENU_ITEM("Change Group");
    MENU_ITEM("Send File");
    MENU_ITEM("Remove Contact");
  }
 
  QString selection;
  MENU_EXEC(selection);
  
  
  //qDebug() << selection;
  
  if (contact->presence == Protocol::Enumerables::Contact::Unaffiliated) {
    if (selection == "Accept") 
    {
      /* TODO need this function implemented*/
      QSet<QString> groups;// = getGroup();
      Dialog::AllowSubscription allow (contact->inviteMessage, contact->nickname, groups, this);
      
      /*TODO check that they don't make duplicate nicknames*/
      if (allow.exec() == QDialog::Accepted) {
        
        mxit.allowSubscription(contact->contactAddress, allow.getGroup(), allow.getNickname());
        emit sendLog("GUI::Contacts "+contact->nickname+" subscribed to");
        /* should get a contacts update back from network after this is sent*/
      }
      //else do nothing
        
    }
    else if (selection == "Reject") 
    {
      QMessageBox sure;
      sure.setText("Are you sure you wish to reject \""+contact->nickname+"\"");
      if (contact->inviteMessage != "")
        sure.setInformativeText(contact->nickname+" sent you an invite message: \"" + contact->inviteMessage+"\"");
        
      sure.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
      sure.setDefaultButton(QMessageBox::Cancel);
      
      if (sure.exec() == QMessageBox::Ok) {
        /* send rejection to client*/
        mxit.denySubscription(contact->contactAddress);
        emit sendLog("GUI::Contacts "+contact->nickname+" subscription denyed");
        addressBook.removeContact(contact->contactAddress);
      }
    }
    else if (selection == "Reject Permanently") 
    {
      QMessageBox sure;
      sure.setText("Are you sure you wish to reject permanently \""+contact->nickname+"\"");
      QString informativeText = "In order to unblock, you need to add "+contact->nickname+" as a contact";
      if (contact->inviteMessage != "")
        informativeText+= "\n"+contact->nickname+" sent you an invite message: \"" + contact->inviteMessage+"\"";
        
      sure.setInformativeText(informativeText);
      sure.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
      sure.setDefaultButton(QMessageBox::Cancel);
      
      if (sure.exec() == QMessageBox::Ok) {
        /* send permanent rejection to client*/
        mxit.denySubscription(contact->contactAddress, true);
        emit sendLog("GUI::Contacts "+contact->nickname+" subscription permanently denyed");
        addressBook.removeContact(contact->contactAddress);
      }
    }
  }
  else 
  {
    if (selection == "Chat") 
    {
      emit chatRequest(lwi);
    }
    else if (selection == "Change Nickname") {
      /* TODO */
    }
    else if (selection == "Change Group") {
      /* TODO */
    }
    else if (selection == "Send File") {
      /* TODO */
    }
    else if (selection == "Remove Contact") {
      
      QMessageBox sure;
      sure.setText("Are you sure you wish to remove \""+contact->nickname+"\"");
      sure.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
      sure.setDefaultButton(QMessageBox::Cancel);
      
      if (sure.exec() == QMessageBox::Ok) {
        mxit.removeContact(contact->contactAddress);
        emit sendLog("GUI:: contact \""+contact->contactAddress+"\" removed");
        addressBook.removeContact(contact->contactAddress);
      }
    }
  }
}


#undef MENU_START
#undef MENU_ITEM
#undef MENU_EXEC

/****************************************************************************
**
** Author: Richard Baxter
**
**
****************************************************************************/

void Contacts::contactsUpdated(const ContactList& contacts) {
  refresh(addressBook.getContacts());
}

/****************************************************************************
**
** Author: Richard Baxter
**
**
****************************************************************************/

void Contacts::refresh(const OrderedContactMap& contacts) {

  QSet<QListWidgetItem*> shouldBeInList;
  
  contactsList->clear();

  /* adding/updating contacts that should be in list*/
  Q_FOREACH(Contact* contact, contacts.values()) {
  
    QListWidgetItem* itemToAdd;
  
    if (contactToLwi.contains(contact)) {
      /* then this contact already has an associated lwi*/
      /* just add it into the new list (done after since all the stuff is there)*/
      
      /* performed after */
      itemToAdd = contactToLwi[contact];
      //contactsList->addItem(contactAddressToLwi[contact.contactAddress]);
    }
    else {
      /* then this contact is new to the list and doesn't have an associated lwi */
      /* create a new lwi and add to hashs and then list (done after since all the stuff is there)*/
      itemToAdd = new QListWidgetItem();
      
      /* add to lookup */
      contactToLwi[contact] = itemToAdd;
      /* add to reverse lookup*/
      lwiToContact[itemToAdd] = contact;
    }
    /* updating listWidgetItem's lable and pixmap*/
    *itemToAdd = QListWidgetItem();
    refreshListWidgetItem(itemToAdd);
    
    contactsList->addItem(itemToAdd);
    shouldBeInList.insert(itemToAdd);
  }
  
  
  /* scanning through list items to look for ones that should not be in their */
  for (int i = 0 ; i < contactsList->count() ; i++) {
    QListWidgetItem * lwi = contactsList->item(i);
    
    if (shouldBeInList.contains(lwi)) {
      /* then the lwi should be in the list */
      /* do nothing */
    }
    else {
      /* then the lwi should NOT be in the list */
      /* remove it and clean up */
      contactsList->removeItemWidget(lwi);
      contactToLwi.remove(lwiToContact[lwi]);
      lwiToContact.remove(lwi);
      delete lwi;
      i--; /*since all the indexes above will have shifted down one and the next item will have index i now*/
    }
  }
}




} /* end of DockWidget namespace */


} /* end of GUI namespace */

} /* end of MXit namespace */





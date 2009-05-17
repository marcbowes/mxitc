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

#include "gui/dialogs/update_contact_info.h"

#include "gui/dialogs/start_group_chat.h"


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
Contacts::Contacts(QWidget* parent, Theme &theme, MXit::Client& mxit, AddressBook & addressBook, MXit::Conversations & conversations) : MXitDockWidget(parent, theme), mxit(mxit), addressBook(addressBook), conversations(conversations)
{
  setupUi(this);
  
  connect(  &addressBook, SIGNAL( updated(const ContactList&)),
            this, SLOT (contactsUpdated(const ContactList&)));
  
  connect(
        contactsTree, 
        SIGNAL( customContextMenuRequested ( const QPoint & ) ), 
        this, 
        SLOT( popUpContextMenu( const QPoint & ) )  );
        
        
  connect(
        contactsTree, 
        SIGNAL( itemDoubleClicked ( QTreeWidgetItem *, int ) ),
        this, 
        SLOT( emitConversationRequest( QTreeWidgetItem *, int ) )  );
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
  for (int i = 0 ; i < contactsTree->topLevelItemCount() ; i++) {
    QTreeWidgetItem * groupTwi = contactsTree->topLevelItem(i);
    
    for (int i = 0 ; i < groupTwi->childCount ()  ; i++) {
      QTreeWidgetItem * twi = groupTwi->child(i);
      refreshTreeWidgetItem(twi);
        
    }
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
** pops up a context menu
**
****************************************************************************/


void Contacts::popUpContextMenu(const QPoint & point) {
  
  
  enum MenuType {UNDEFINED, SINGLE, UNAFFILIATED, GROUP, MULTI, NONE};
  MenuType menuType = UNDEFINED;
  
  QTreeWidgetItem * twi = (contactsTree->itemAt ( point.x(), point.y() ));
  QList<QTreeWidgetItem *> selectedItems =  contactsTree->selectedItems ();
  
  /* ======== selecting which menu to use ======== */
  
  if (selectedItems.size() == 0) {
    menuType = NONE;
    return; /* TODO is there a contect menu for nothing */
  }
  else if (selectedItems.size() > 1) {
    menuType = MULTI;
  }
  else { //  if (selectedItems.size() == 1)
    if (twi) {
      if (twiToGroup.contains(twi)) {
        /*then this is a group item*/
        menuType = GROUP;
      }
      else {
        /*then this is not a group icon => contact item*/
        if (twiToContact.value(twi)->presence == Protocol::Enumerables::Contact::Unaffiliated)
          menuType = UNAFFILIATED;
        else
          menuType = SINGLE;
      }
    }
    else {
      menuType = NONE;
    }
  }
  
  /* ======== defining menus ======== */
  
  void (Contacts::*menuFunction)(const QString&, const QList<QTreeWidgetItem *>&  ) = NULL;
  QList<QString> menuItems;
  QString title;
  
  
  if (menuType == SINGLE && (menuFunction = &Contacts::singleContactMenu)) {
    title = twiToContact[selectedItems.front()]->nickname;
    menuItems.append("Chat");
    menuItems.append("Change Nickname or Group");
    menuItems.append("Send File");
    menuItems.append("Remove Contact");
  }
  else if (menuType == UNAFFILIATED && (menuFunction = &Contacts::unaffiliatedContactMenu)) {
    title = twiToContact[selectedItems.front()]->nickname;
    menuItems.append("Accept");
    menuItems.append("Reject");
    menuItems.append("Reject Permanently");
  }
  else if (menuType == GROUP && (menuFunction = &Contacts::groupContactMenu)) {
    title = twiToGroup.value(twi);
    if (twiToGroup[twi] != "")
      menuItems.append("Change Group Name");
      
    menuItems.append("Send Message To Group");
    menuItems.append("Start Group Chat");
    menuItems.append("Remove Group");
  }
  else if (menuType == MULTI && (menuFunction = &Contacts::multiContactMenu)) {
    title = "Multiple Contacts";
    menuItems.append("Start Group Chat");
    menuItems.append("Change Group");
    menuItems.append("Remove Contacts");
  }
  else if (menuType == NONE && (menuFunction = NULL)) {
  }
  else if (menuType == UNDEFINED && (menuFunction = NULL)) {
  }
  
  /* running correct context menu */

  if (menuFunction != NULL) {
    QMenu contextMenu(title, this);
    QHash<QString, QAction*> hash;
    Q_FOREACH (const QString& str, menuItems) {
      hash[str] = new QAction (str, this); 
      contextMenu.addAction(hash[str]);
    }
    QString selectedString = "";
    QAction* a = contextMenu.exec( contactsTree->mapToGlobal(point) );
    bool nothingSelected = true;
    if (a){ 
      selectedString = a->text(); 
      nothingSelected = false;
    }
    Q_FOREACH(QAction * act, hash){ 
      delete act; 
    }
    if (nothingSelected) 
      return;
    else {
      (this->*menuFunction)(selectedString, selectedItems);
      
    }
  }
}


/****************************************************************************
  _____          __          __    __  ___                
 / ___/__  ___  / /______ __/ /_  /  |/  /__ ___  __ _____
/ /__/ _ \/ _ \/ __/ -_) \ / __/ / /|_/ / -_) _ \/ // (_-<
\___/\___/_//_/\__/\__/_\_\\__/ /_/  /_/\__/_//_/\_,_/___/

                          
****************************************************************************/

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contacts::undefinedContactMenu(const QString & selection, const QList<QTreeWidgetItem *>& selectedTwi) {

}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contacts::singleContactMenu(const QString & selection, const QList<QTreeWidgetItem *>& selectedTwi) {

  QTreeWidgetItem * twi = selectedTwi.front();
  Contact * contact = twiToContact.value(twi);
  if (selection == "Chat") 
  {
    emitConversationRequest(twi);
  }
  else if (selection == "Change Nickname or Group") {
    Dialog::UpdateContactInfo update(contact->nickname, getGroupNames(), contact->group, this);
    /*TODO check that they don't make duplicate nicknames*/
    if (update.exec() == QDialog::Accepted) {
      
      /*wait for implementsation in addressbook*/
      addressBook.updateContact(update.getGroup(), contact->contactAddress, update.getNickname());
      mxit.updateContactInfo(update.getGroup(), contact->contactAddress, update.getNickname());
      emit sendLog("GUI::Contacts "+contact->nickname+" updated to "+update.getNickname() +" group " + update.getGroup());
      /* should get a contacts update back from network after this is sent*/
      /* NOTE, tested this, reply does NOT come...rage*/
    }
      
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
      /*removing from gui*/
      QSet<const Conversation*> involvements = conversations.getInvolvements(contact);
      Q_FOREACH(const Conversation* conversation, involvements) {
        if (conversation->type == Protocol::Enumerables::Message::Normal)
          conversations.toggleActive(conversation->uniqueIdentifier);
      }
      removeAndDeleteContactOrGroupFromGUI(twi);
    }
  }

}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/


void Contacts::unaffiliatedContactMenu(const QString & selection, const QList<QTreeWidgetItem *>& selectedTwi) {
  QTreeWidgetItem * twi = selectedTwi.front();
  Contact * contact = twiToContact.value(twi);
  if (selection == "Accept") 
  {
    /* TODO need this function implemented*/
    Dialog::AllowSubscription allow (contact->inviteMessage, contact->nickname, getGroupNames(), this);
    
    /*TODO check that they don't make duplicate nicknames*/
    if (allow.exec() == QDialog::Accepted) {
      
      mxit.allowSubscription(contact->contactAddress, allow.getGroup(), allow.getNickname());
      emit sendLog("GUI::Contacts "+contact->nickname+" subscribed to");
      /* should get a contacts update back from network after this is sent*/
    }
    //else do nothing
      
  }
  else if (selection == "Reject" || selection == "Reject Permanently") 
  {
    bool block = (selection == "Reject Permanently");
    QString informativeText = "";
    
    QMessageBox sure;
    sure.setText("Are you sure you wish to reject \""+contact->nickname+"\""+ (block?" permanently":"")+"?");
    
    if (block) {
      informativeText+="In order to unblock, you need to add "+contact->nickname+" as a contact";
    }
    
    if (contact->inviteMessage != "") {
      if (block) informativeText+="\n";
      informativeText+=contact->nickname+" sent you an invite message: \"" + contact->inviteMessage+"\"";
    }
    
    
    sure.setInformativeText(informativeText);
      
    sure.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    sure.setDefaultButton(QMessageBox::Cancel);
    
    if (sure.exec() == QMessageBox::Ok) {
      /* send rejection to client*/
      mxit.denySubscription(contact->contactAddress, block);
      emit sendLog("GUI::Contacts "+contact->nickname+" subscription"+(block?" permanently":"")+" denyed");
      addressBook.removeContact(contact->contactAddress);
      removeAndDeleteContactOrGroupFromGUI(twi);
    }
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/


void Contacts::groupContactMenu(const QString & selection, const QList<QTreeWidgetItem *>& selectedTwi) {

  if (selection == "Change Group Name") {
    /*TODO*/
  }
  else if (selection == "Send Message To Group") {
    ContactList contactList = genContactsListFromTwiSelection(selectedTwi);
    //mxit->sendGroupMessage(const QString &group, const ContactList &contacts, const QString &message, Protocol::Enumerables::Message::Type type, unsigned int flags); /*TODO WTF do these mean!??*/
  }
  else if (selection == "Start Group Chat") {
    ContactList contactList = genContactsListFromTwiSelection(selectedTwi);
    createNewGroupChat(contactList);
  }
  else if (selection == "Remove Group") {
    /*TODO*/
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contacts::multiContactMenu(const QString & selection, const QList<QTreeWidgetItem *>& selectedTwi) {

  if (selection == "Start Group Chat") {
    /**/
    ContactList contactList = genContactsListFromTwiSelection(selectedTwi);
    createNewGroupChat(contactList);
    /*TODO*/
  }
  else if (selection == "Change Group") {
    /*TODO*/
  }
  else if (selection == "Remove Contacts") {
    /*TODO*/
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contacts::createNewGroupChat(const ContactList& contactList) {

  Dialog::StartGroupChat start;
  
  QStringList stringList;
  Q_FOREACH (const Contact * contact, contactList) { stringList.append(contact->nickname); }
  
  QString lastName = stringList.back(); stringList.pop_back();
  
  qDebug() << contactList.size();
  QString names = (contactList.size() != 1?stringList.join(", ") + QString(" and ") + lastName:lastName);
  start.setText("Select a room name for your chat with "+names);
  
  if (start.exec() == QDialog::Accepted) {
    mxit.createNewGroupChat(start.getRoomname(), contactList);
    emit sendLog("GUI::Contacts groupchat \""+start.getRoomname()+"\" created");
      
    /*FIXME cant switch to new group chat since it doesn't exist yet*/
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/


ContactList Contacts::genContactsListFromTwiSelection(const QList<QTreeWidgetItem *>& selectedTwi) {

  ContactList contactList;
  Q_FOREACH(QTreeWidgetItem * twi, selectedTwi) {
    if (twiToGroup.contains(twi)) {
      /* add all contacts in group*/
      for (int i = 0 ; i < twi->childCount () ; i++)
        contactList.append(twiToContact.value(twi->child(i)));
    }
    else
      contactList.append(twiToContact.value(twi));
  
  }
  return contactList;
}


/****************************************************************************
**
** Author: Richard Baxter
**
** emits a conversation request with a pointer to the appropriate contact
**
****************************************************************************/

void Contacts::emitConversationRequest(QTreeWidgetItem *item, int index) {
  emit conversationRequest(twiToContact.value(item));
}


/****************************************************************************
**
** Author: Richard Baxter
**
** returns an ordered list of group names
**
****************************************************************************/

QStringList Contacts::getGroupNames() {
  return orderedGroupNames.keys();
}

/****************************************************************************
**
** Author: Richard Baxter
**
** refreshes a treeWidgetItem
**
****************************************************************************/

void Contacts::refreshTreeWidgetItem(QTreeWidgetItem *item) {

  if (twiToGroup.contains(item)) {
    /* then this is a group item*/
    QString name = (twiToGroup.value(item) == ""?QString("Ungrouped"):twiToGroup.value(item));
    item->setText(0, "Group: "+name+"");
  }
  else {
    /*then this is a contact item*/
    Contact * contact = twiToContact.value(item);
    item->setIcon(0, theme.contact.presence.pixmap(contact->presence));
    item->setText(0, contact->nickname);
    
      
    item->setForeground(0, contact->presence == Protocol::Enumerables::Contact::Unaffiliated?Qt::blue:Qt::black);
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contacts::contactsUpdated(const ContactList& contacts) {
  refresh(addressBook.getContacts());
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contacts::refresh(const OrderedContactMap& contacts) {

  QSet<QTreeWidgetItem*> shouldBeInList;
  QSet<QTreeWidgetItem*> groupShouldBeInList;
  QSet<QTreeWidgetItem*> groupAlreadyInGui;
  
  QSet<QTreeWidgetItem*> expandedItems;
  for (int j = 0 ; j < contactsTree->topLevelItemCount() ; j++) {
    QTreeWidgetItem * groupTwi = contactsTree->topLevelItem(j);
    //qDebug() << groupTwi->text(0) << " expanded? " << groupTwi->isExpanded();
    if (groupTwi->isExpanded())
      expandedItems.insert(groupTwi);
  }
  
  /*clear tree*/
  while(contactsTree->topLevelItemCount()) {
    QTreeWidgetItem * groupTwi = contactsTree->topLevelItem(0);
    
    while(groupTwi->childCount ()) {
      groupTwi->takeChild(0);
    }
    
    contactsTree->takeTopLevelItem ( 0 );
    
  }
  
  orderedGroupNames.clear();

  /* adding/updating contacts that should be in list*/
  Q_FOREACH(Contact* contact, contacts.values()) {
    
    /*adding group*/
    {
      QTreeWidgetItem* groupTreeItemToAdd = NULL;
      
      //qDebug() << "groupToTwi.contains("<<contact->group<<")" << groupToTwi.contains(contact->group);
      if (groupToTwi.contains(contact->group)) {
        /* then this group already has an associated twi*/
        /* just add it into the new list (done after the if statement since all the stuff is there)*/
        
        /* add performed after */
        groupTreeItemToAdd = groupToTwi.value(contact->group);
      }
      else {
        /* then this group is new to the tree and doesn't have an associated twi */
        /* create a new twi and add to hashs and then tree (done after the if statement since all the stuff is there)*/
        groupTreeItemToAdd = new QTreeWidgetItem();
        expandedItems.insert(groupTreeItemToAdd);
        
        /* add to lookup */
        groupToTwi[contact->group] = groupTreeItemToAdd;
        /* add to reverse lookup*/
        twiToGroup[groupTreeItemToAdd] = contact->group;
        
        
      }
      
      if (groupAlreadyInGui.contains(groupTreeItemToAdd)) {
        /* then the group has been added to the GUI*/
        /* do nothing */
      } 
      else {
        /* then the group has NOT been added to the GUI*/
        
        
        //qDebug() << contact->group <<" added to GUI";
        refreshTreeWidgetItem(groupTreeItemToAdd);
        
        groupShouldBeInList.insert(groupTreeItemToAdd);
        
        groupAlreadyInGui.insert(groupTreeItemToAdd);
        
        orderedGroupNames[twiToGroup[groupTreeItemToAdd]] = true;
      }
    }
    /* adding contact*/
    {
      QTreeWidgetItem* treeItemToAdd = NULL;
      //qDebug() << contact->presence << "contactToTwi.contains("<<contact->nickname<<")" << contactToTwi.contains(contact);
      if (contactToTwi.contains(contact)) {
        /* then this contact already has an associated twi*/
        /* just add it into the new list (done after the if statement  since all the stuff is there)*/
        
        /* add performed after */
        treeItemToAdd = contactToTwi.value(contact);
        
      }
      else {
        /* then this contact is new to the tree and doesn't have an associated twi */
        /* create a new lwi and add to hashs and then tree (done after the if statement  since all the stuff is there)*/
        treeItemToAdd = new QTreeWidgetItem();
        
        /* add to lookup */
        contactToTwi[contact] = treeItemToAdd;
        /* add to reverse lookup*/
        twiToContact[treeItemToAdd] = contact;
        
      }
      
      bool hideOfflineContacts = false; /*TODO*/
      if(!hideOfflineContacts) {
        //qDebug() << "adding " << contact->nickname;
        /* updating listWidgetItem's lable and pixmap*/
        refreshTreeWidgetItem(treeItemToAdd);
        
        shouldBeInList.insert(treeItemToAdd);
        groupToTwi[contact->group]->addChild (treeItemToAdd);
        //shouldBeInTree.insert(treeItemToAdd);
      }
    }
    
  }
  
  
  /* scanning through list items to look for ones that should not be in their */
  for (int j = 0 ; j < contactsTree->topLevelItemCount() ; j++) {
    QTreeWidgetItem * groupTwi = contactsTree->topLevelItem(j);
    
    if (groupShouldBeInList.contains(groupTwi)) {
      /* then the group twi should be in the list and is*/
      
      
      /* scan through it to check if there are any 'old' twi 's to remove */
      for (int i = 0 ; i < groupTwi->childCount ()  ; i++) {
        QTreeWidgetItem * twi = groupTwi->child(i);
        
        if (shouldBeInList.contains(twi)) {
          /* then the twi should be in the list but is */
          /* do nothing */
        }
        else {
          /* then the twi should NOT be in the list but is */
          /* remove it and clean up */
        
          removeAndDeleteContactOrGroupFromGUI (twi);
          i--; /*since all the indexes above will have shifted down one and the next item will have index i now*/
        }
      } 
    }
    else {
      /* then the group twi should NOT be in the list but is */
      
      /* remove it and clean up */
      removeAndDeleteContactOrGroupFromGUI (groupTwi);
      j--; /*since all the indexes above will have shifted down one and the next item will have index j now*/
    }
  }
  
  while(contactsTree->topLevelItemCount()) {
    contactsTree->takeTopLevelItem ( 0 );
  }
  
  Q_FOREACH(const QString & group, orderedGroupNames.keys()) {
    contactsTree->addTopLevelItem ( groupToTwi.value(group) );
    // contactsTree->addTopLevelItem(groupTreeItemToAdd);
    groupToTwi.value(group)->setExpanded (expandedItems.contains(groupToTwi.value(group)));
  
  }
  
  
  
  //qDebug() <<orderedGroupNames; /*TODO test groupMap thing*/
  emit groupsUpdated(getGroupNames());
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Contacts::removeAndDeleteContactOrGroupFromGUI (QTreeWidgetItem * twi) {
  
  if (twiToGroup.contains(twi)) {
    /* then this is a group item*/
    contactsTree->removeItemWidget(twi, 0);
    groupToTwi.remove(twiToGroup[twi]);
    twiToGroup.remove(twi);
    delete twi;
  }
  else {
    /* this is a contact item */
    contactsTree->removeItemWidget(twi, 0);
    contactToTwi.remove(twiToContact[twi]);
    twiToContact.remove(twi);
    delete twi;
  }
}



} /* end of DockWidget namespace */


} /* end of GUI namespace */

} /* end of MXit namespace */





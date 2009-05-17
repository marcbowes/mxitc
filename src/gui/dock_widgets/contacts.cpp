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
    QTreeWidgetItem * twi = contactsTree->topLevelItem(i);
    
    refreshTreeWidgetItem(twi);
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

void Contacts::popUpContextMenu(const QPoint & point) {
  
  QTreeWidgetItem * twi = (contactsTree->itemAt ( point.x(), point.y() ));
  Contact * contact;
  QPoint pos;
  qDebug() << twi;
  if (twi) {
    pos = contactsTree->mapToGlobal ( point );
    if (twiToGroup.contains(twi)) {
      /*then this is a group item*/
      contact = NULL;
    }
    else {
      /*then this is not a group icon => contact item*/
      contact = twiToContact.value(twi);
    }
  }
  else {
    return;
    /*TODO - implement NULL context menu -> add new group*/
  }
  
  MENU_START(contact?contact->nickname:twiToGroup.value(twi));
  if (contact) {
  
    if (contact->presence == Protocol::Enumerables::Contact::Unaffiliated) {
      MENU_ITEM("Accept");
      MENU_ITEM("Reject");
      MENU_ITEM("Reject Permanently");
    }
    else {
      MENU_ITEM("Chat");
      MENU_ITEM("Change Nickname or Group");
      MENU_ITEM("Send File");
      MENU_ITEM("Remove Contact");
    }
  }
  else { /*group*/
    
    if (twiToGroup[twi] != "")
      MENU_ITEM("Change Group Name");
      
    MENU_ITEM("Send Message To Group");
    MENU_ITEM("Remove Group");
    
  }
 
  QString selection;
  MENU_EXEC(selection);
  
  
  
  if (contact) {
    if (contact->presence == Protocol::Enumerables::Contact::Unaffiliated) {
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
    else 
    {
      if (selection == "Chat") 
      {
        emitConversationRequest(twi);
      }
      else if (selection == "Change Nickname or Group") {
        Dialog::UpdateContactInfo update(contact->nickname, getGroupNames(), contact->group, this);
        /*TODO check that they don't make duplicate nicknames*/
        if (update.exec() == QDialog::Accepted) {
          
          addressBook.updateContact(contact->contactAddress);
          /*TODO wait for implementation in client*/
          mxit.updateContactInfo(contact->contactAddress, update.getGroup(), update.getNickname());
          emit sendLog("GUI::Contacts "+contact->nickname+" updated to "+update.getNickname() +" group " + update.getGroup());
          /* should get a contacts update back from network after this is sent*/
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
            if (conversation->type == MXit::Conversation::Private)
              conversations.toggleActive(conversation->uniqueIdentifier);
          }
          removeAndDeleteContactOrGroupFromGUI(twi);
        }
      }
    }
  }
  else { /*group*/
    if (selection == "Change Group Name") {
      /*TODO*/
    }
    else if (selection == "Send Message To Group") {
      /*TODO*/
    }
    else if (selection == "Remove Group") {
      /*TODO*/
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
** returns an orderedset of group names
**
****************************************************************************/

const QMap<QString, bool>& Contacts::getGroupNames() {

  return orderedGroupNames;
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
    qDebug() << groupTwi->text(0) << " expanded? " << groupTwi->isExpanded();
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
        
        /* add to lookup */
        groupToTwi[contact->group] = groupTreeItemToAdd;
        /* add to reverse lookup*/
        twiToGroup[groupTreeItemToAdd] = contact->group;
        
        
        orderedGroupNames[twiToGroup[groupTreeItemToAdd]] = true;
        qDebug() << "adding group " << twiToGroup[groupTreeItemToAdd];
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
        contactsTree->addTopLevelItem(groupTreeItemToAdd);
        groupTreeItemToAdd->setExpanded (expandedItems.contains(groupTreeItemToAdd));
        
        groupAlreadyInGui.insert(groupTreeItemToAdd);
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
      //qDebug() << "adding " << contact->nickname;
      /* updating listWidgetItem's lable and pixmap*/
      refreshTreeWidgetItem(treeItemToAdd);
      
      shouldBeInList.insert(treeItemToAdd);
      groupToTwi[contact->group]->addChild (treeItemToAdd);
      //shouldBeInTree.insert(treeItemToAdd);
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
      
      qDebug() << "removing group " << twiToGroup[groupTwi];
      orderedGroupNames.remove(twiToGroup[groupTwi]);
      /* remove it and clean up */
      removeAndDeleteContactOrGroupFromGUI (groupTwi);
      j--; /*since all the indexes above will have shifted down one and the next item will have index j now*/
    }
  }
  
  qDebug() <<orderedGroupNames; /*TODO test groupMap thing*/
  emit groupsUpdated(orderedGroupNames);
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





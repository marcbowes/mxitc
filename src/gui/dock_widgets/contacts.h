/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Richard Baxter, 2009
**
****************************************************************************/

#ifndef __MXIT_GUI_DOCKWIDGET_CONTACTS_H__
#define __MXIT_GUI_DOCKWIDGET_CONTACTS_H__

#include "mxit/address_book.h"
#include "mxit/conversations.h"
#include "mxit/contact.h"
#include "mxit/client.h"
#include "options.h"

#include <QTreeWidget>

#include "gui/mxit_dock_widget.h"

#include "ui_contacts.h"

namespace MXit
{

namespace GUI
{

namespace DockWidget
{

class Contacts : public MXitDockWidget, private Ui::ContactsDockWidget
{
  Q_OBJECT
  
  public: /*class specific */

  Contacts(QWidget* parent, Theme &theme, MXit::Client& mxit, AddressBook & addressBook, MXit::Conversations & conversations, Options & options);
  ~Contacts();
  
  
  signals:
  void outgoingConversationShowRequest ( const Contact * contact );
  
  void groupsUpdated( const QStringList& groupNames);
  
  public: /* methods */
  
  QStringList getGroupNames(); /* recreates list... slow! FIXME ?*/
  
  void clearList();
  
  private: /* methods */
  
  void refreshList(const OrderedContactMap& contacts);
  void refreshTreeWidgetItem(QTreeWidgetItem* twi);
  void removeAndDeleteContactOrGroupFromGUI (QTreeWidgetItem* twi);
  
  
  void undefinedContactMenu     (const QString & selection, const QList<QTreeWidgetItem *>& selectedTwi);
  void unaffiliatedContactMenu  (const QString & selection, const QList<QTreeWidgetItem *>& selectedTwi);
  void groupContactMenu         (const QString & selection, const QList<QTreeWidgetItem *>& selectedTwi);
  void multiContactMenu         (const QString & selection, const QList<QTreeWidgetItem *>& selectedTwi);
  void singleContactMenu        (const QString & selection, const QList<QTreeWidgetItem *>& selectedTwi);
  
  
  QString genContactListString(const ContactList& contactList);
  void createNewGroupChat(const ContactList& contactList);
  void changeGroupName(const ContactList& contactList);
  ContactList genContactsListFromTwiSelection(const QList<QTreeWidgetItem *>& selectedTwi);
  
  public slots:
  void refreshTheming();
 

  private slots:
  
  void emitConversationShowRequest (QTreeWidgetItem* twi, int index = 0);
  
  void contactsUpdated(const ContactList& contacts);
  void popUpContextMenu(const QPoint & pos);
  
  
  private: /* variables */
  
  MXit::Conversations& conversations;
  AddressBook& addressBook;
  MXit::Client& mxit;
  Options& options;
  
  QMap<QString, bool> orderedGroupNames;
  
  QHash<QString, QTreeWidgetItem*> groupToTwi;
  QHash<QTreeWidgetItem*, QString> twiToGroup;
  QHash<MXit::Contact*, QTreeWidgetItem*> contactToTwi;
  QHash<QTreeWidgetItem*, MXit::Contact*> twiToContact;
  
  

};

} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif



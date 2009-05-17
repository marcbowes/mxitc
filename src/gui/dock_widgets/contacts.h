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

  Contacts(QWidget* parent, Theme &theme, MXit::Client& mxit, AddressBook & addressBook, MXit::Conversations & conversations);
  ~Contacts();
  
  
  signals:
  void conversationRequest ( const Contact * contact );
  
  void groupsUpdated( const QMap<QString, bool>& groupNames);
  
  public: /* methods */
  
  const QMap<QString, bool>& getGroupNames();
  void refresh(const OrderedContactMap& contacts);
  
  private: /* methods */
  
  void refreshTreeWidgetItem(QTreeWidgetItem* twi);
  void removeAndDeleteContactOrGroupFromGUI (QTreeWidgetItem* twi);
  
  
  void undefinedContactMenu     (const QString & selection, const QList<QTreeWidgetItem *>& selectedTwi);
  void unaffiliatedContactMenu  (const QString & selection, const QList<QTreeWidgetItem *>& selectedTwi);
  void groupContactMenu         (const QString & selection, const QList<QTreeWidgetItem *>& selectedTwi);
  void multiContactMenu         (const QString & selection, const QList<QTreeWidgetItem *>& selectedTwi);
  void singleContactMenu        (const QString & selection, const QList<QTreeWidgetItem *>& selectedTwi);
  
  public slots:
  void refreshThemeing();
 

  private slots:
  
  void emitConversationRequest (QTreeWidgetItem* twi, int index = 0);
  
  void contactsUpdated(const ContactList& contacts);
  void popUpContextMenu(const QPoint & pos);
  
  
  private: /* variables */
  
  MXit::Conversations& conversations;
  AddressBook& addressBook;
  MXit::Client& mxit;
  
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



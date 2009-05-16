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
  
  //void outgoingItemPressed ( QListWidgetItem *  ); // depricated maybe
  
  signals:
  void conversationRequest ( const Contact * );
  
  public: /* methods */
  
  void refresh(const OrderedContactMap& contacts);
  
  private: /* methods */
  
  void refreshListWidgetItem(QListWidgetItem* lwi);
  void removeAndDeleteContactFromGUI (QListWidgetItem * lwi);

  public slots:
  void refreshThemeing();

  private slots:
  
  void emitConversationRequest (QListWidgetItem* lwi);
  
  void contactsUpdated(const ContactList& contacts);
  void popUpContextMenu(const QPoint & pos);
  
  
  private: /* variables */
  
  MXit::Conversations& conversations;
  AddressBook& addressBook;
  MXit::Client& mxit;
  
  /* TODO sohuld be const Contact* */
  QHash<MXit::Contact*, QListWidgetItem*> contactToLwi;
  QHash<QListWidgetItem*, MXit::Contact*> lwiToContact;
  

};

} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif



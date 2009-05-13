/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Richard Baxter, 2009
**
****************************************************************************/

#ifndef __MXIT_GUI_DOCKWIDGET_CONTACTS_H__
#define __MXIT_GUI_DOCKWIDGET_CONTACTS_H__

#include "mxit/contact.h"

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

  Contacts(QWidget* parent, Theme &theme);
  ~Contacts();
  
  signals:
  
  void outgoingItemPressed ( QListWidgetItem *  );
  
  void contextMenuRequest(const QPoint &, const QString& nickname);
  
  signals:
  void chatRequest ( QListWidgetItem * );
  
  public:
  
  void refresh(const QList<MXit::Contact>& contacts);

  private slots:
  
  void contactsListContextMenuRequest(const QPoint & pos);
  
  
  private:
  
  //void clearList();
  QListWidgetItem * addContact(const MXit::Contact & c);
  
  QHash<QString, QListWidgetItem*> listItemWidgets; // from nickname to listItemWidget
  

};

} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif



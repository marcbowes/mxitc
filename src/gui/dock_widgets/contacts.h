/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::OptionsDockWidget provides a login window.
** This is an extension of the generated dialog, made with Designer.
**
****************************************************************************/

#ifndef __MXIT_GUI_DOCKWIDGET_CONTACTS_H__
#define __MXIT_GUI_DOCKWIDGET_CONTACTS_H__

#include "gui/mxit_dock_widget.h"

#include "ui_contacts.h"

#include <QDebug>

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

  Contacts(QWidget* parent = 0);
  ~Contacts();
  
  signals:
  
  void outgoingItemPressed ( QListWidgetItem *  );
  
  public:
  
  void clearList();
  void addItemToList(const QListWidgetItem & liw);


};

} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif



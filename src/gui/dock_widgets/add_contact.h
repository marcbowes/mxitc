/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Richard Baxter, 2009
**
****************************************************************************/

#ifndef __MXIT_GUI_DOCKWIDGET_ADDCONTACT_H__
#define __MXIT_GUI_DOCKWIDGET_ADDCONTACT_H__

#include "gui/mxit_dock_widget.h"

#include "ui_add_contact.h"

#include "protocol/enumerables/contact.h"

namespace MXit
{

namespace GUI
{

namespace DockWidget
{

class AddContact : public MXitDockWidget, private Ui::AddContactDockWidget
{
  Q_OBJECT
  
  public: /*class specific */

  AddContact(QWidget* parent, Theme &theme);
  ~AddContact();
  
  signals:
  
  void addContact(const QString &group, const QString &contactAddress, const QString &nickname,
    Protocol::Enumerables::Contact::Type type, const QString &message);
  
  private slots:
  
  void networkChanged ( int index );
  void sendAddContactInfo();
  


};

} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif



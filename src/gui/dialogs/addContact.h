/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::Dialog::AddContact provides a dialog window to add a contact
** This is an extension of the generated dialog, made with Designer.
**
****************************************************************************/

#ifndef __MXIT_GUI_DIALOG_ADD_CONTACT_H__
#define __MXIT_GUI_DIALOG_ADD_CONTACT_H__

#include <QMessageBox>

#include "mxit/client.h"

#include "ui_addContact.h"

namespace MXit
{

namespace GUI
{

namespace Dialog
{

class AddContact : public QDialog, private Ui::AddContact
{
  Q_OBJECT

  public:         /* class specific */
  
  AddContact(QWidget *parent = 0, MXit::Client *client = 0);
  ~AddContact();

  private slots:
  
  private:        /* variables */
  
  MXit::Client *mxit; /* FIXME should this be here, if so, should this not be generalised out of all the dialogs?*/
};

} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */
 
#endif


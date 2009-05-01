/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::Dialog::AddContact provides a dialog window to add a contact
** This is an extension of the generated dialog, made with Designer.
**
****************************************************************************/

#ifndef __MXIT_GUI_DIALOG_DEBUG_DIALOG_H__
#define __MXIT_GUI_DIALOG_DEBUG_DIALOG_H__


#include "gui/mxit_dialog.h"

#include "ui_debug_dialog.h"

namespace MXit
{

namespace GUI
{

namespace Dialog
{

class DebugDialog : public MXitDialog, private Ui::DebugDialog
{
  Q_OBJECT

  public:         /* class specific */
  
  DebugDialog(QWidget *parent = 0, MXit::Client *client = 0, QSettings* settings = 0);
  ~DebugDialog();

  private slots:
  
};


} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */
 
#endif


/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "debug_dialog.h"

namespace MXit
{

namespace GUI
{

namespace Dialog
{


/****************************************************************************
**
** Author: Richard Baxter
**
** Dialog constructor
**
****************************************************************************/
DebugDialog::DebugDialog(QWidget *parent, MXit::Client *client, QSettings* settings) : MXitDialog (parent, client, settings)
{
  setupUi(this);      /* from ui_dialog.h: generated from dialog.ui */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Dialog deconstructor
**
****************************************************************************/
DebugDialog::~DebugDialog()
{
  // nothing here
}


} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */


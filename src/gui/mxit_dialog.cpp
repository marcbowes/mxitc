/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "mxit_dialog.h"

namespace MXit
{

namespace GUI
{

/****************************************************************************
**
** Author: Richard Baxter
**
** Dialog constructor
**
****************************************************************************/
MXitDialog::MXitDialog(QWidget *parent, MXit::Client *client, QSettings* settings) : QDialog (parent), settings(settings), mxit (client)
{
}


/****************************************************************************
**
** Author: Richard Baxter
**
** Dialog deconstructor
**
****************************************************************************/
MXitDialog::~MXitDialog()
{
  // nothing here
}



} /* end of GUI namespace */

} /* end of MXit namespace */


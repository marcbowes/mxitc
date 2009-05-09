

#include "options.h"

namespace MXit
{

namespace GUI
{

namespace DockWidget
{



/****************************************************************************
**
** Author: Richard Baxter
**
** Widget constructor
**
****************************************************************************/
Options::Options(QWidget* parent) : MXitDockWidget(parent)
{
  setupUi(this);
  
  connect(httpRadioButton, SIGNAL(toggled ( bool )), httpWidget, SLOT(setEnabled ( bool )));
  
  connect(httpRadioButton, SIGNAL(toggled ( bool )), this, SIGNAL(gatewaySelected( bool )));
}



/****************************************************************************
**
** Author: Richard Baxter
**
** Widget destructor
**
****************************************************************************/
Options::~Options()
{

}


} /* end of DockWidget namespace */


} /* end of GUI namespace */

} /* end of MXit namespace */





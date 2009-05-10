

#include "log.h"

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
Log::Log(QWidget* parent, Theme &theme) : MXitDockWidget(parent, theme)
{
  setupUi(this);
  
}



/****************************************************************************
**
** Author: Richard Baxter
**
** Widget destructor
**
****************************************************************************/
Log::~Log()
{

}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Log::logMessage(const QString& msg) {
  listWidget->addItem ( msg );
}


} /* end of DockWidget namespace */


} /* end of GUI namespace */

} /* end of MXit namespace */





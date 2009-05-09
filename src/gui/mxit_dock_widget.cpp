/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "mxit_dock_widget.h"

namespace MXit
{

namespace GUI
{

/****************************************************************************
**
** Author: Richard Baxter
**
** DockWindow constructor
**
****************************************************************************/
MXitDockWidget::MXitDockWidget(QWidget *parent) : QDockWidget (parent)
{
}


/****************************************************************************
**
** Author: Richard Baxter
**
** DockWindow deconstructor
**
****************************************************************************/
MXitDockWidget::~MXitDockWidget()
{
  // nothing here
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void MXitDockWidget::toggleVisibility() {
  
  /*TODO make a trigger raise if not raised else show if not shown else not show if shown( and raised)*/
  /*qDebug() << widget->isVisible();
  if (!widget->isActiveWindow()) {
    widget->show();
    widget->raise();
    widget->activateWindow();
  }
  else {*/
  
  
  if (isVisible()) {
    setVisible(false);
  }
  else {
    setVisible(true);
    raise();
  }
  
  //}
}


} /* end of GUI namespace */

} /* end of MXit namespace */


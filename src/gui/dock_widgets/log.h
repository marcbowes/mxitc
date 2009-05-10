/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Richard Baxter, 2009
**
****************************************************************************/

#ifndef __MXIT_GUI_DOCKWIDGET_LOG_H__
#define __MXIT_GUI_DOCKWIDGET_LOG_H__

#include "gui/mxit_dock_widget.h"

#include "ui_log.h"

namespace MXit
{

namespace GUI
{

namespace DockWidget
{

class Log : public MXitDockWidget, private Ui::LogDockWidget
{
  Q_OBJECT
  
  public: /*class specific */

  Log(QWidget* parent, Theme &theme);
  ~Log();
  
  signals:
  
  
  public:
  
  void logMessage(const QString&);


};

} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif



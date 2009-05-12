/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Richard Baxter, 2009
**
****************************************************************************/

#ifndef __MXIT_GUI_DOCKWIDGET_DEBUG_H__
#define __MXIT_GUI_DOCKWIDGET_DEBUG_H__

#include <QDockWidget>
#include <QHash>

#include "common/types.h"

#include "gui/mxit_dock_widget.h"

#include "ui_debug.h"

namespace MXit
{

namespace GUI
{

namespace DockWidget
{



class Debug : public MXitDockWidget, private Ui::DebugDockWidget
{
  Q_OBJECT
  
  public: /*class specific */

  Debug(QWidget* parent, Theme &theme);
  ~Debug();
  
  signals:
  void requestVariableHashRefresh();

  public slots:
  
  void incomingVariableHash(const VariableHash& variables);
  
  
  private slots:
  void refreshRequested();


};


} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif



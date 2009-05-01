/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::Login provides a login window.
** This is an extension of the generated dialog, made with Designer.
**
****************************************************************************/

#ifndef __MXIT_GUI_DEBUG_WIDGET_H__
#define __MXIT_GUI_DEBUG_WIDGET_H__

#include <QDockWidget>

#include "ui_debugWidget.h"

namespace MXit
{

namespace GUI
{


class DebugDockWidget : public QDockWidget, private Ui::DebugDockWidget
{
  Q_OBJECT
  
  public: /*class specific */

  DebugDockWidget(QWidget* parent = 0);
  ~DebugDockWidget();



};


} /* end of GUI namespace */

} /* end of MXit namespace */



#endif


/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::OptionsDockWidget provides a login window.
** This is an extension of the generated dialog, made with Designer.
**
****************************************************************************/

#ifndef __MXIT_GUI_DOCKWIDGET_OPTIONS_H__
#define __MXIT_GUI_DOCKWIDGET_OPTIONS_H__

#include "gui/mxit_dock_widget.h"

#include "ui_options.h"

namespace MXit
{

namespace GUI
{

namespace DockWidget
{

class Options : public MXitDockWidget, private Ui::OptionsDockWidget
{
  Q_OBJECT
  
  public: /*class specific */

  Options(QWidget* parent, Theme &theme);
  ~Options();
  
  signals:
  void gatewaySelected(bool http);
  
  public slots:
  
  
  private slots:


};

} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif



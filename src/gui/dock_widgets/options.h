/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Richard Baxter, 2009
**
****************************************************************************/

#ifndef __MXIT_GUI_DOCKWIDGET_OPTIONS_H__
#define __MXIT_GUI_DOCKWIDGET_OPTIONS_H__

#include <QFileDialog>

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
  
  
  public: /*method */
  
  QString getBaseThemeDirectory();
  void setBaseThemeDirectory(const QString& dir);
  QString getSelectedTheme();
  void setSelectedTheme(const QString& theme);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  signals:
  void gatewaySelected(bool http);
  
  signals:
  void themeChanged();
  
  
  public slots:
  
  
  private slots:
  void openThemeBrowser ();
  void refreshComboBox ();
  void loadTheme(const QString & dir);


};

} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif



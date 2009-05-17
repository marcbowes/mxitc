/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Richard Baxter, 2009
**
****************************************************************************/

#ifndef __MXIT_GUI_DOCKWIDGET_OPTIONS_H__
#define __MXIT_GUI_DOCKWIDGET_OPTIONS_H__

#include <QDir>
#include <QFileDialog>
#include <QSettings>

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

  Options(QWidget* parent, Theme &theme, QSettings& settings);
  ~Options();
    
  public: /*method */
  
  void addGateway(const QString& gateway);
  void setSelectedGateway(const QString& gateway);
  
  QString getBaseThemeDirectory();
  void setBaseConversationsDirectory(const QString& dir);
  void setBaseThemeDirectory(const QString& dir);
  QString getSelectedTheme();
  void setSelectedTheme(const QString& theme);
  
  
  signals:
  
  void conversationLogDirectorySelected(const QDir &dir);
  void gatewaySelected(const QString &gateway, const QString &proxyHost, const QString &proxyPort);
  
  void themeChanged();
  
  
  private slots:
  
  void emitGatewaySignal ();
  void saveGatewaySettings(bool nothing = false);
  
  void openConversationsBrowser ();
  void openThemeBrowser ();
  void refreshComboBox ();
  void loadTheme(const QString & dir);
  void reloadCurrentTheme();
  
  private: /* variables */
  
  QSettings& settings;


};

} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif



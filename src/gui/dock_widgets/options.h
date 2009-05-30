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
#include "mxit/client.h"

#include "ui_options.h"

namespace MXit
{

namespace GUI
{

namespace DockWidget
{

class Options : public MXitDockWidget, public Ui::OptionsDockWidget
{
  Q_OBJECT
  
  public: /*class specific */

  Options(QWidget* parent, Theme &theme, MXit::Client& mxit, QSettings& settings);
  ~Options();
    
  public: /*method */
  
  /*accessors TODO make a variableHash*/
  bool isAutoLogin();
  bool hideOfflineContacts();
  
  void addGateway(const QString& gateway);
  void setSelectedGateway(const QString& gateway);
  
  QString getBaseConversationsDirectory();
  void setBaseConversationsDirectory(const QString& dir);
  QString getBaseThemeDirectory();
  void setBaseThemeDirectory(const QString& dir);
  QString getSelectedTheme();
  void setSelectedTheme(const QString& theme);
  void reAdd();
  
  
  private slots:
  void saveSettings( );
  
  void incomingVariables(const VariableHash& variables);
  
  
  signals:
  
  void conversationLogDirectorySelected(const QDir &dir);
  
  void themeChanged( );
  void requestRefresh( );
  
  
  private slots:
  
  void setGatewayInClient ();
  void saveGatewaySettings();
  void saveThemeOptionsSettings();
  
  void openConversationsBrowser ();
  void openThemeBrowser ();
  void refreshComboBox ();
  void loadTheme(const QString & dir);
  void reloadCurrentTheme();
  
  private: /* variables */
  
  QSettings& settings;
  MXit::Client& mxit;
  bool loadingSettings;


};

} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif



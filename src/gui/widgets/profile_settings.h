/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Richard Baxter, 2009
**
****************************************************************************/

#ifndef __MXIT_GUI_WIDGET_PROFILESETTINGS_H__
#define __MXIT_GUI_WIDGET_PROFILESETTINGS_H__


#include "ui_profile_settings.h"

namespace MXit
{

namespace GUI
{

namespace Widget
{

class ProfileSettings : public QWidget, public Ui::ProfileSettingsWidget
{
  Q_OBJECT
  
  public: /*class specific */

  ProfileSettings(QWidget* parent = 0);
  ~ProfileSettings();
  
  signals:
  
  
  public slots:


};

} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif



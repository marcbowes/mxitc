/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Richard Baxter, 2009
**
****************************************************************************/

#ifndef __MXIT_GUI_DOCKWIDGET_PROFILE_H__
#define __MXIT_GUI_DOCKWIDGET_PROFILE_H__

#include "gui/mxit_dock_widget.h"
#include "gui/widgets/profile_settings.h"
#include "mxit/client.h"


#include "ui_profile.h"


namespace MXit
{

namespace GUI
{

namespace DockWidget
{

class Profile : public MXitDockWidget, private Ui::ProfileDockWidget
{
  Q_OBJECT
  
  public: /*class specific */

  Profile(QWidget* parent, Theme &theme, MXit::Client& mxit);
  ~Profile();
  
  private slots:
  void sendProfileChange ();
  void checkEnabledOfApplyButton();

  private: /* methods */
  
  bool checkIfApplyPressable();

  private:
  MXit::Client& mxit;
  
  Widget::ProfileSettings* profileSettings;

};

} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */



#endif



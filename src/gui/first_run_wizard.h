#ifndef __MXIT_GUI_FIRSTRUNWIZARD_H__
#define __MXIT_GUI_FIRSTRUNWIZARD_H__


#include "gui/dock_widgets/options.h"
#include "mxit/client.h"

#include "ui_first_run_wizard.h"


namespace MXit
{

namespace GUI
{


class FirstRunWizard: public QWizard, private Ui::FirstRunWizard {


  public:
  FirstRunWizard(DockWidget::Options &options);
  ~FirstRunWizard();

  public: /* methods */
  
  bool registerSelected();

  private:
  DockWidget::Options &options;

};

  
} /* end of GUI namespace */

} /* end of MXit namespace */






#endif /* __MXIT_GUI_FIRSTRUNWIZARD_H__ */


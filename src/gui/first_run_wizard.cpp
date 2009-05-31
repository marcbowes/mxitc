


#include "first_run_wizard.h"



namespace MXit
{

namespace GUI
{


FirstRunWizard::FirstRunWizard(DockWidget::Options &options): QWizard(), options(options) {
  setupUi(this);
  
  generalPage->layout()->addWidget(options.generalWidget);
  themePage->layout()->addWidget(options.themeWidget);
  conversationsPage->layout()->addWidget(options.conversationsWidget);
  
}


FirstRunWizard::~FirstRunWizard() {
  options.reAdd();
}



bool FirstRunWizard::registerSelected() {
  return notRegisterdButton->isChecked();
}


} /* end of GUI namespace */

} /* end of MXit namespace */

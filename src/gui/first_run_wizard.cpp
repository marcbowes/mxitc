


#include "first_run_wizard.h"



namespace MXit
{

namespace GUI
{


FirstRunWizard::FirstRunWizard(MXit::Client& mxit, DockWidget::Options &options): QWizard(), options(options) {
  setupUi(this);
  
  mxit.initialize();
  //gatewayFrame->layout()->addWidget(widget ( ) );
  gatewayPage->layout()->addWidget(options.gatewayWidget);
  generalPage->layout()->addWidget(options.generalWidget);
  themAndLogPage->layout()->addWidget(options.themeWidget);
  themAndLogPage->layout()->addWidget(options.conversationsWidget);
  
}


FirstRunWizard::~FirstRunWizard() {
  options.reAdd();

}



} /* end of GUI namespace */

} /* end of MXit namespace */

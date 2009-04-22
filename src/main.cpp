#include "gui/mxitc.h"



int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  MXit::Client mxit;
  MXit::GUI::MXitC mxitc(&app, &mxit);
  
  
  
  mxitc.show();

  return app.exec();
}

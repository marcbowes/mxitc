#include "gui/mxitc.h"
#include <QApplication>

#ifdef WIN32
  #include <QtPlugin>
  
  Q_IMPORT_PLUGIN(qgif)
  Q_IMPORT_PLUGIN(qjpeg)
#endif

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  MXit::Client mxit;
  MXit::GUI::MXitC mxitc(&app, &mxit);
  
  
  mxitc.show();

  return app.exec();
}

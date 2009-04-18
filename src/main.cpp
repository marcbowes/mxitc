#include "gui/login.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	MXit::Client mxit;
  MXit::GUI::Login login(&app, &mxit);
  login.show();

	return app.exec();
}

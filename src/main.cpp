#include "gui/login.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
  MXit::GUI::Login login(&app);
  login.show();

	return app.exec();
}

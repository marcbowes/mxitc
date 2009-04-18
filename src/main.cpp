#include "gui/dialog.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
  MXit::GUI::Dialog dialog(&app);
  dialog.show();

	return app.exec();
}

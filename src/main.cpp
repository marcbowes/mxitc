#include "dialog.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
  Dialog dialog(&app);
  dialog.show();

	return app.exec();
}

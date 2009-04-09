#include <QCoreApplication>
#include <QStringList>
#include "mxit.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc,argv);
	QStringList args = app.arguments();

	Mxit mxit_client;

	return app.exec();
}

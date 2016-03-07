#include "Engine/Engine.h"

#include <QApplication>
#include "Engine/Engine.h"

using namespace Runes;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);


	app.setOrganizationName("Runes");
	app.setApplicationName("Runic spell editor");
	Engine mainWindow;
	mainWindow.init();
	mainWindow.show();

	return app.exec();
}
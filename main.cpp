#include <QtWidgets>
#include "MainWindow.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	app.setApplicationName("Imagine");
	app.setApplicationVersion("1.0");
	app.setOrganizationName("Vladimir Gudkov");
	app.setAttribute(Qt::AA_EnableHighDpiScaling);
	app.setAttribute(Qt::AA_UseHighDpiPixmaps);
	app.setWindowIcon(QIcon(":/icons/app-icon"));
	
	MainWindow w;
	w.show();
	
	return app.exec();
}

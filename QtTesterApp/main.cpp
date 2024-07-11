#include "Logger\Logger.h"
#include "QtTestAppView.h"

int main(int argc, char* argv[])
{
	logging::LogAction(L"main(): Entry Point");
	QApplication a(argc, argv);

	QtMainWindowView w;
	w.show();

	return a.exec();
}
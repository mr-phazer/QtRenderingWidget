#include "..\..\Rldx\Rldx\Logging\Logging.h"
#include "QtTestAppView.h"

int main(int argc, char* argv[])
{

	int  DEBUG_TEST = 1;

	logging::LogAction("main(): Entry Point");
	QApplication a(argc, argv);

	QtMainWindowView w;
	w.show();

	return a.exec();
}

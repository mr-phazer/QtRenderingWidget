#include <CommonLibs\Logger\Logger.h>
#include <QtTesterApp\QtTestAppView.h>

int main(int argc, char* argv[])
{
	logging::LogAction(L"main(): Entry Point");
	QApplication a(argc, argv);

	QtMainWindowView w;
	w.show();

	return a.exec();
}

#include <QtWidgets/QApplication>
#include "QtTestAppView.h"

#include "..\ImportExport\RigidModel\Readers\RigidModelReader.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	QtMainWindowView w;
	w.show();

	return a.exec();
}

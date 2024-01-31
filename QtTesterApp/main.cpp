#include "QtTesterApp.h"
#include <QtWidgets/QApplication>

#include "..\ImportExport\RigidModel\Readers\RigidModelReader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); 

    QtTesterAppWindow w;
    w.show();

    return a.exec();
}

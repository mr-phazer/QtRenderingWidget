#include "QtTesterApp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtTesterApp w;
    w.show();
    return a.exec();
}

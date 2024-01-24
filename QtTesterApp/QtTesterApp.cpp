#include "QtTesterApp.h"

#include "..\QtRenderingWidget\ExternFunctions\Creators.h"
#include <qlayout.h>


QtTesterApp::QtTesterApp(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    auto renderWidget = CreateQRenderingWidget(this);

    if (!renderWidget)
        return;

    this->setCentralWidget(renderWidget);   
    setWindowTitle("QRenderingWidget pluging for RPFM - Tester App");
}

QtTesterApp::~QtTesterApp()
{}

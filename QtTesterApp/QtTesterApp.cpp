#include "QtTesterApp.h"

#include "..\QtRenderingWidget\ExternFunctions\Creators.h"
#include <qlayout.h>


QtTesterApp::QtTesterApp(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    // TODO: clean up, maybe but some of this in controller
    QString gameIdString = "wh2";
    auto renderWidget = CreateQRenderingWidget(this, &gameIdString, nullptr);

    if (!renderWidget)
        return;

    this->setCentralWidget(renderWidget);   
    setWindowTitle("QRenderingWidget pluging for RPFM - Tester App");
}

QtTesterApp::~QtTesterApp()
{}

#include "QtTesterApp.h"

#include "..\QtRenderingWidget\ExternFunctions\Creators.h"
#include <qlayout.h>


QtTesterApp::QtTesterApp(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    auto renderWidget = CreateQRenderingWidget(this);

    this->setCentralWidget(renderWidget);   
    
}

QtTesterApp::~QtTesterApp()
{}

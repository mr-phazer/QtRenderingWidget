#include "QtTesterApp.h"

#include "..\QtRenderingWidget\ExternFunctions\Creators.h"
#include "..\ImportExport\Helpers\ByteStream.h"
#include <qlayout.h>


QtTesterAppWindow::QtTesterAppWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    // TODO: clean up, maybe but some of this in controller
    QString gameIdString = "wh2";
    auto renderWidget = CreateQRenderingWidget(this, &gameIdString, nullptr);
    
    if (!renderWidget)
        return;

    ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1\emp\emp_karl_franz\emp_karl_franz.rigid_model_v2)");
    //ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1d\def\def_malekith\def_malekith_body_01.rigid_model_v2)");

    QString fileName = QString::fromStdWString(bytes.GetPath().c_str());
    QByteArray qBytes((char*)bytes.GetBufferPtr(), bytes.GetBufferSize());
    AddNewPimaryAsset(renderWidget, &fileName, &qBytes);
    

    this->setCentralWidget(renderWidget);   
    setWindowTitle("QRenderingWidget pluging for RPFM - Tester App");
}

QtTesterAppWindow::~QtTesterAppWindow()
{}

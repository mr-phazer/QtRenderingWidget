#include "QtTesterApp.h"

#include "..\QtRenderingWidget\ExternFunctions\Creators.h"
#include "..\ImportExport\Helpers\ByteStream.h"
#include <qlayout.h>


QtTesterAppWindow::QtTesterAppWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
        
    // TODO: maybe not have this constructor !!!! even for test program
    Init();

    setWindowTitle("QRenderingWidget pluging for RPFM - Tester App");
}

void QtTesterAppWindow::Init()
{      
    QString gameIdString = "wh2";
    auto renderWidget = CreateQRenderingWidget(this, &gameIdString, nullptr);

    if (!renderWidget)
        return;

    //ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1\emp\emp_props\emp_karl_franz_hammer_2h_01.rigid_model_v2)");
    //ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1\emp\emp_karl_franz\emp_karl_franz.rigid_model_v2)");
    //ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1d\def\def_malekith\def_malekith_body_01.rigid_model_v2)");
    ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1\emp\emp_props\emp_free_company_sword_1h_01.rigid_model_v2)");

    QString fileName = QString::fromStdWString(bytes.GetPath().c_str());
    QByteArray qBytes((char*)bytes.GetBufferPtr(), bytes.GetBufferSize());

    AddNewPimaryAsset(renderWidget, &fileName, &qBytes);

    this->setCentralWidget(renderWidget);
 
}

QtTesterAppWindow::~QtTesterAppWindow()
{}

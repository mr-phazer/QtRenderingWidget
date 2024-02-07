#include "QtMainWindowView.h"

#include <qlayout.h>

#include "..\QtRenderingWidget\ExternFunctions\Creators.h"
#include "..\ImportExport\Helpers\ByteStream.h"
#include "..\QtRenderingWidget\Constants\GameIdKeys.h"

struct TestData
{
    std::wstring assetFolder = LR"(I:\Modding\WH3\)";
    std::string gameId = game_id_keys::KEY_WARHAMMER_3;
    std::wstring filePath = LR"(I:\Modding\WH3\variantmeshes\wh_variantmodels\hu1\ksl\ksl_katarin\ksl_katarin_01.rigid_model_v2)";
}
testData_WH3_Person;

TestData testData_WH3_Weapon =
{
    LR"(I:\Modding\WH3\)",
    game_id_keys::KEY_WARHAMMER_3,
    LR"(I:\Modding\WH3\variantmeshes\wh_variantmodels\hu1\ksl\ksl_props\ksl_archer_sword_1h_01.rigid_model_v2)"
};


QtMainWindowView::QtMainWindowView(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
        
    // TODO: maybe not have this constructor !!!! even for test program
    InitRenderView();

    setWindowTitle("QRenderingWidget pluging for RPFM - Tester App");
}

void QtMainWindowView::InitRenderView()
{   

    auto ptestData = &testData_WH3_Person;
    QString gameIdString = QString::fromStdString(ptestData->gameId);

    auto renderWidget = CreateQRenderingWidget(this, &gameIdString, nullptr);

    auto qAssetPath = QString::fromStdWString(ptestData->assetFolder);    
    SetAssetFolder(&qAssetPath);
   
    if (!renderWidget)
        return;

    //ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1\emp\emp_props\emp_karl_franz_hammer_2h_01.rigid_model_v2)");
    //ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1\emp\emp_karl_franz\emp_karl_franz.rigid_model_v2)");
    //ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1d\def\def_malekith\def_malekith_body_01.rigid_model_v2)");
    //ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1\emp\emp_props\emp_free_company_sword_1h_01.rigid_model_v2)");
     
    // - Rome 2 test models 
    ByteStream bytes(ptestData->filePath);

    QString fileName = QString::fromStdWString(bytes.GetPath().c_str());
    QByteArray qBytes((char*)bytes.GetBufferPtr(), bytes.GetBufferSize());

    AddNewPrimaryAsset(renderWidget, &fileName, &qBytes);

    this->setCentralWidget(renderWidget);    
}

QtMainWindowView::~QtMainWindowView()
{}

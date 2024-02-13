#include "QtMainWindowView.h"

#include <qlayout.h>

#include "..\QtRenderingWidget\ExternFunctions\Creators.h"
#include "..\ImportExport\Helpers\ByteStream.h"
#include "..\QtRenderingWidget\Constants\GameIdKeys.h"
#include "..\Rldx\Rldx\Managers\ResourceManager\DxResourceManager.h"
#include "..\Rldx\Rldx\Managers\ResourceManager\DxResourceByteStream.h"

/// <summary>
/// Debugging data struct, made to unclutter the code
/// </summary>
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

TestData testData_WH3_Person_Malekith =
{
	LR"(I:\Modding\WH3\)",
	game_id_keys::KEY_WARHAMMER_3,
	LR"(I:\Modding\WH3\variantmeshes\wh_variantmodels\hu1d\def\def_malekith\def_malekith_body_01.rigid_model_v2)"
};

TestData testData_WH3_Prop_missing_specglos
{
	LR"(I:\Modding\WH3\)",
	game_id_keys::KEY_WARHAMMER_3,
	LR"(I:\Modding\WH3\variantmeshes\wh_variantmodels\hu1\emp\emp_props\emp_amber_wizard_staff_01.rigid_model_v2)"
};

TestData testData_WH3_Building_Mean_to_not_work_yet =
{
	LR"(I:\Modding\WH3\)",
	game_id_keys::KEY_WARHAMMER_3,
	LR"(I:\Modding\WH3\rigidmodels/buildings/brt_seacliff_a/brt_seacliff_a_piece01_destruct01.rigid_model_v2)"
};

TestData testData_WH3_Building_missing_texture =
{
	LR"(I:\Modding\WH3\)",
	game_id_keys::KEY_WARHAMMER_3,
	LR"(I:\Modding\WH3\rigidmodels\buildings\chd_chimney_small_01a\chd_chimney_small_01a_piece01_destruct01.rigid_model_v2)"
};

TestData testData_WH3_Terrain_Made_To_Crash =
{
	LR"(I:\Modding\WH3\)",
	game_id_keys::KEY_WARHAMMER_3,
	LR"(I:/Modding/WH3/terrain/tiles/campaign/roads_light/2x2_straight_br_tl_b/river_mesh.wsmodel.rigid_model_v2)"
};

TestData testData_WH3_Terrain_Made_To_Crash2 =
{
	LR"(I:\Modding\WH3\)",
	game_id_keys::KEY_WARHAMMER_3,
	LR"(I:/Modding/WH3/terrain/tiles/battle/dar_river/wh3_main_macro_dar_river_01/tile_12/river_mesh.wsmodel.rigid_model_v2)"
};

TestData testData_Rome2_Armor =
{
	LR"(K:\Modding\Rome2\Vanilla\)",
	game_id_keys::KEY_ROME_2,
	LR"(K:\Modding\Rome2\Vanilla\variantmeshes\_variantmodels\man\armour\lorica_segmentata_c.rigid_model_v2)"
};


QtMainWindowView::QtMainWindowView(QWidget* parent)
	: QMainWindow(parent)
{
	setupUi(this);

	// TODO: maybe not have this constructor !!!! even for test program
	InitRenderView();

	setWindowTitle("TWViewer 0.0.1a - Total War Viewing/Conversion Tool");
}

void QtMainWindowView::InitRenderView()
{

	// TODO: remove debuggin code
#ifdef NOT_DEBUG
	auto newResHandle = rldx::DxResourceManager::Instance()->AllocEmpty<rldx::DxResourceByteStream>(L"myASS");
	newResHandle.GetPtr()->byteStream = ByteStream(testData_WH3_Person.filePath);

	auto resHandler = rldx::DxResourceManager::Instance()->GetResourceByString<rldx::DxResourceByteStream>(L"MYass");

#endif

	auto ptestData = &testData_WH3_Weapon;
	QString gameIdString = QString::fromStdString(ptestData->gameId);

	auto renderWidget = CreateQRenderingWidget(this, &gameIdString, nullptr);

	auto qAssetPath = QString::fromStdWString(ptestData->assetFolder);
	SetAssetFolder(&qAssetPath);

	if (!renderWidget)
		return;

	ByteStream bytes(ptestData->filePath);
	QString fileName = QString::fromStdWString(bytes.GetPath().c_str());
	QByteArray qBytes((char*)bytes.GetBufferPtr(), bytes.GetBufferSize());
	QString outErrorString;

	AddNewPrimaryAsset(renderWidget, &fileName, &qBytes, &outErrorString);

	PauseRendering(renderWidget);
	ResumeRendering(renderWidget);
	
	// TODO: remove - test code for making "free windows"
	//renderWidget->setWindowFlag(Qt::WindowType::Window, true);
	//renderWidget->resize(1024, 1024);
	//renderWidget->show();

	this->setCentralWidget(renderWidget);
}

QtMainWindowView::~QtMainWindowView()
{}

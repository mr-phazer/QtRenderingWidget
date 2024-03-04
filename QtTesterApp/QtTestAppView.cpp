#include "QtTestAppView.h"

#include <qlayout.h>

#include "..\ImportExport\Helpers\ByteStream.h"
#include "..\QtRenderingWidget\Constants\GameIdKeys.h"
#include "..\QtRenderingWidget\ExternFunctions\Creators.h"
#include "..\Rldx\Rldx\Managers\ResourceManager\DxResourceByteStream.h"
#include "..\Rldx\Rldx\Managers\ResourceManager\DxResourceManager.h"


#include "..\ImportExport\FileFormats\Anim\Creators\AnimFileHeaderCreator.h"

/// <summary>
/// Debugging data struct, made to unclutter the code
/// </summary>
struct TestData
{
	std::wstring assetFolder = LR"(I:\Modding\WH3\)";
	std::wstring gameId = game_id_keys::KEY_WARHAMMER_3;
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

TestData testData_WH3_WSMODEL
{
	LR"(I:\Modding\WH3\)",
	game_id_keys::KEY_WARHAMMER_3,
	LR"(I:\Modding\WH3\variantmeshes\wh_variantmodels\wf1\chd\chd_wolf\chd_wolf_02.wsmodel)"
};

TestData testData_WH3_Person_Malekith_Wrong_Folder =
{
	LR"(I:\Modding\WH3\WRONG\WRONG\)",
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
	LR"(I:\Modding\Rome2\)",
	game_id_keys::KEY_ROME_2,
	LR"(K:\Modding\Rome2\Vanilla\variantmeshes\_variantmodels\man\armour\lorica_segmentata_c.rigid_model_v2)"
};

TestData testData_Rome2_VMD =
{
	LR"(I:\Modding\Rome2\)",
	game_id_keys::KEY_ROME_2,
	LR"(I:\Modding\Rome2\variantmeshes\variantmeshdefinitions\roman_legionary.variantmeshdefinition)"
};

TestData testData_Attila_VMD_legion_comi
{
	LR"(I:\Modding\Attila\)",
	game_id_keys::KEY_ATTILA,
	LR"(I:\Modding\Attila\variantmeshes\variantmeshdefinitions\att_rom_legio_comitatenses_east.variantmeshdefinition)"
};

TestData testData_Attila_VMD_spearlegion
{
	LR"(I:\Modding\Attila\)",
	game_id_keys::KEY_ATTILA,
	LR"(I:\Modding\Attila\variantmeshes\variantmeshdefinitions\att_rom_comitatensis_spears.variantmeshdefinition)"
};

TestData testData_WH3_VMD
{
	LR"(I:\Modding\WH3\)",
	game_id_keys::KEY_WARHAMMER_3,
	LR"(I:\Modding\WH3\variantmeshes\variantmeshdefinitions\wef_wildwood_rangers_ror.variantmeshdefinition)"
};

TestData testData_WH3__chs_knights___VMD
{
	LR"(I:\Modding\WH3\)",
	game_id_keys::KEY_WARHAMMER_3,
	LR"(I:\Modding\WH3\variantmeshes\variantmeshdefinitions\chs_knights.variantmeshdefinition)"
};


QtMainWindowView::QtMainWindowView(QWidget* parent)
	: QMainWindow(parent)
{
	setupUi(this);

	// TODO: maybe not have this constructor !!!! even for test program
	InitRenderView_DEBUG();

	setWindowTitle("TWViewer 0.0.1a - Total War Viewing/Conversion Tool");

	resize(1525, 1525);
}

// TODO: REMOVE and cleanup
void QtMainWindowView::InitRenderView_DEBUG()
{

	// TODO: remove debuggin code
#ifdef _DEBUG

	ByteStream bytesForAnim(LR"(K:\Modding\WH2\animations\battle\bigcat01\stand\bc1_stand_01.anim)");
	Anim_V7_HeaderFileCommonCreator animHeaderCreator;
	auto testHeaderCOmmon = animHeaderCreator.Create(bytesForAnim);


#endif	


	auto ptestData = &testData_WH3__chs_knights___VMD;
	auto qAssetPath = QString::fromStdWString(ptestData->assetFolder);
	rldx::DxResourceManager::SetGameAssetFolder(qAssetPath.toStdWString());
	QString gameIdString = QString::fromStdWString(ptestData->gameId);

	QString globalAssetFolder = QString::fromStdWString(LR"(I:\Coding\Repos\QtRenderingWidget_RPFM\x64\Debug)");
	SetAssetFolder(&globalAssetFolder);

	QString globalLogFolder = QString::fromStdWString(LR"(c:\temp\)");
	SetLogFolder(&globalLogFolder);

	auto renderWidget = CreateQRenderingWidget(this, &gameIdString, nullptr);

	if (!renderWidget)
		return;

	ByteStream bytes(ptestData->filePath);
	QString fileName = QString::fromStdWString(bytes.GetPath().c_str());
	QByteArray qBytes((char*)bytes.GetBufferPtr(), bytes.GetBufferSize());
	QString outErrorString;

	//AddNewPrimaryAsset(renderWidget, &fileName, &qBytes, &outErrorString);
	TESTCODE_AddNewPrimaryAsset(renderWidget, &fileName, &qBytes, &outErrorString);

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

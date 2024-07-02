#pragma once

#include <string>
#include "..\QtRenderingWidget\Constants\GameIdKeys.h"

namespace test_app_data
{

	const std::wstring assetFolder_WH3 = LR"(H:\Modding\WH3\)";
	const std::wstring assetFolder_Attila = LR"(I:\Modding\Attila\)";
	const std::wstring assetFolder_WH2 = LR"(k:\Modding\WH2)";

	/// <summary>
	/// Debugging data struct, made to unclutter the code
	/// </summary>
	struct TestData
	{
		TestData() = default;
		TestData(const std::wstring& inAssetFolder, const std::wstring& inGameIdString, const std::wstring& inFilePath)
			:
			assetFolder(inAssetFolder),
			gameId(inGameIdString),
			filePath(inFilePath)
		{}

		std::wstring assetFolder = assetFolder_WH3;
		std::wstring gameId = game_id_keys::KEY_WARHAMMER_3;
		std::wstring filePath = LR"(I:\Modding\WH3\variantmeshes\wh_variantmodels\hu1\ksl\ksl_katarin\ksl_katarin_01.rigid_model_v2)";
	};

	TestData testData_WH3_Person;

	TestData testData_WH3_Weapon =
	{
		assetFolder_WH3,
		game_id_keys::KEY_WARHAMMER_3,
		LR"(I:\Modding\WH3\variantmeshes\wh_variantmodels\hu1\ksl\ksl_props\ksl_archer_sword_1h_01.rigid_model_v2)"
	};

	TestData testData_WH3_Person_Malekith =
	{
		assetFolder_WH3,
		game_id_keys::KEY_WARHAMMER_3,
		LR"(I:\Modding\WH3\variantmeshes\wh_variantmodels\hu1d\def\def_malekith\def_malekith_body_01.rigid_model_v2)"
	};

	TestData testData_WH3_WSMODEL
	{
		assetFolder_WH3,
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
		assetFolder_WH3,
		game_id_keys::KEY_WARHAMMER_3,
		LR"(I:\Modding\WH3\variantmeshes\wh_variantmodels\hu1\emp\emp_props\emp_amber_wizard_staff_01.rigid_model_v2)"
	};

	TestData testData_WH3_Building_Mean_to_not_work_yet =
	{
		assetFolder_WH3,
		game_id_keys::KEY_WARHAMMER_3,
		LR"(I:\Modding\WH3\rigidmodels/buildings/brt_seacliff_a/brt_seacliff_a_piece01_destruct01.rigid_model_v2)"
	};

	TestData testData_WH3_Building_missing_texture =
	{
		assetFolder_WH3,
		game_id_keys::KEY_WARHAMMER_3,
		LR"(I:\Modding\WH3\rigidmodels\buildings\chd_chimney_small_01a\chd_chimney_small_01a_piece01_destruct01.rigid_model_v2)"
	};

	TestData testData_WH3_Terrain_Made_To_Crash =
	{
		assetFolder_WH3,
		game_id_keys::KEY_WARHAMMER_3,
		LR"(I:/Modding/WH3/terrain/tiles/campaign/roads_light/2x2_straight_br_tl_b/river_mesh.wsmodel.rigid_model_v2)"
	};

	TestData testData_WH3_Terrain_Made_To_Crash2 =
	{
		assetFolder_WH3,
		game_id_keys::KEY_WARHAMMER_3,
		LR"(I:/Modding/WH3/terrain/tiles/battle/dar_river/wh3_main_macro_dar_river_01/tile_12/river_mesh.wsmodel.rigid_model_v2)"
	};

	TestData testData_WH2_Knight_VMD =
	{
		LR"(K:\Modding\WH2\)",
		game_id_keys::KEY_WARHAMMER_2,
		LR"(K:\Modding\WH2\variantmeshes\variantmeshdefinitions\brt_knights_of_the_realm.variantmeshdefinition)"
	};

	TestData testData_WH2_GrailKnight_VMD =
	{
		LR"(K:\Modding\WH2\)",
		game_id_keys::KEY_WARHAMMER_2,
		LR"(K:\Modding\WH2\variantmeshes\variantmeshdefinitions\brt_grail_knights.variantmeshdefinition)"
	};

	TestData testData_WH2_brt_grail_guardians_VMD =
	{
		LR"(K:\Modding\WH2\)",
		game_id_keys::KEY_WARHAMMER_2,
		LR"(K:\Modding\WH2\variantmeshes\variantmeshdefinitions\brt_grail_guardians.variantmeshdefinition)"
	};

	TestData testData_WH3_brt_grail_guardians_VMD =
	{
		assetFolder_WH3,
		game_id_keys::KEY_WARHAMMER_3,
		LR"(H:\Modding\WH3\variantmeshes\variantmeshdefinitions\brt_grail_guardians.variantmeshdefinition)"
	};

	TestData testData_WH3_RMV2_
	{
		assetFolder_WH3,
		game_id_keys::KEY_WARHAMMER_3,
		LR"(H:\modding\WH3\variantmeshes\wh_variantmodels\bc2\emp\emp_demigryph\body\emp_demigryph_body_01.rigid_model_v2)"
	};

	TestData testData_WH3_RMV2_KARL
	{
		assetFolder_WH3,
		game_id_keys::KEY_WARHAMMER_3,
		LR"(H:\modding\WH3\variantmeshes\wh_variantmodels\hu1\emp\emp_karl_franz\emp_karl_franz.rigid_model_v2)"
	};

	TestData testData_WH3_WSMODL
	{
		assetFolder_WH3,
		game_id_keys::KEY_WARHAMMER_3,
		LR"(H:\modding\WH3\variantmeshes\wh_variantmodels\hu1_t1\cst\cst_deck_gunners\cst_deck_gunners_back_body_01.wsmodel)"
	};

	TestData WH2_battlePilgrims_VMD =
	{
		LR"(K:\Modding\WH2\)",
		game_id_keys::KEY_WARHAMMER_2,
		LR"(K:\Modding\WH2\variantmeshes\variantmeshdefinitions\brt_battle_pilgrims.variantmeshdefinition)"
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
		assetFolder_WH3,
		game_id_keys::KEY_WARHAMMER_3,
		LR"(I:\Modding\WH3\variantmeshes\variantmeshdefinitions\wef_wildwood_rangers_ror.variantmeshdefinition)"
	};

	TestData testData_Attila__Tiled__VicTest
	{
		LR"(I:\Modding\Attila\)",
		game_id_keys::KEY_ATTILA,
		LR"(C:\Users\45311\Documents\sassanid_fort_straight_piece01_destruct01.rigid_model_v2)"
	};

	TestData testData_Attila__ShipAmbient__VicTest
	{
		LR"(I:\Modding\Attila\)",
		game_id_keys::KEY_ATTILA,
		LR"(C:\Users\45311\Documents\sassanid_fort_straight_piece01_destruct01.rigid_model_v2)"
	};
}
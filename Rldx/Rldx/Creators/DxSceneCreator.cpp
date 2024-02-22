#include "DxSceneCreator.h"

#include "..\..\QtRenderingWidget\Constants\GameIdKeys.h"

#include "..\Creators\DxGameShaderCreators.h"
#include "..\Creators\DxModelNodeCreators\DxModelNodeCreator.h"
#include "..\..\..\ImportExport\WsModel\Reader\WsModelReader.h"

#include "..\SceneGraph\SceneNodes\DxVmdNode.h"

rldx::DxScene::Uptr rldx::DxSceneCreator::InitNewScene(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, bool isSRGB, const std::string& name)
{
	m_newScene = std::make_unique<DxScene>(name);

	SetWindowPos(m_nativeWindowHandle, nullptr, 0, 0, 2000, 1024, SWP_NOOWNERZORDER);
	RECT windowRect;
	GetWindowRect(m_nativeWindowHandle, &windowRect);

	UINT width = windowRect.right - windowRect.left;
	UINT height = windowRect.bottom - windowRect.top;

	// create swap chain
	m_newScene->GetRefSwapChain() = DxSwapChain::CreateForHWND(poDevice, m_nativeWindowHandle, isSRGB, width, height);
	m_newScene->InitRenderView(poDevice);

	//newScene->Resize(poDevice, poDeviceContext, 2000, 1024);

	return std::move(m_newScene);
}

rldx::DxScene::Uptr rldx::DxSceneCreator::Create(HWND nativeWindHandle, ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::string& gameStringId)
{
	m_nativeWindowHandle = nativeWindHandle;
	
	RGBModeEnum rgbMode = RGB_Mode;

	if (gameStringId == game_id_keys::KEY_WARHAMMER ||
		gameStringId == game_id_keys::KEY_WARHAMMER_2 ||
		gameStringId == game_id_keys::KEY_WARHAMMER_3 ||
		gameStringId == game_id_keys::KEY_PHARAOH ||
		gameStringId == game_id_keys::KEY_THREE_KINGDOMS ||
		gameStringId == game_id_keys::KEY_TROY)
	{
		rgbMode = RGBModeEnum::SRGB_Mode;
	}

	m_newScene = InitNewScene(poDevice, poDeviceContext, RGBModeEnum::SRGB_Mode);

	// -- make default, fallback shaders
	auto newSimpleShaderProgram =
		rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
			poDevice,
			/*libtools::GetExePath() + */LR"(VS_Simple.cso)",
			/*libtools::GetExePath() + */LR"(PS_Simple.cso)"
		);

	auto noTextures_ShaderProgram =
		rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
			poDevice,
			/*libtools::GetExePath() + */LR"(VS_Simple.cso)",
			/*libtools::GetExePath() + */LR"(PS_NoTextures.cso)"
		);

	// make grid node, mesh, fill node, set shaders
	auto meshNodeGrid = rldx::DxMeshNode::Create("Grid");
	auto gridMeshData = rldx::DxMeshCreatorHelper::MakeGrid(poDevice, 40, 0.1f);

	meshNodeGrid->SetModelData(gridMeshData);
	meshNodeGrid->SetShaderProgram(newSimpleShaderProgram);
	m_newScene->GetRootNode()->AddChild(meshNodeGrid);
	
	DxResourceManager::Instance()->SetDefaultShaderProgram(noTextures_ShaderProgram);	

	return std::move(m_newScene);
}

void rldx::DxSceneCreator::TESTCODE_AddVMD(ID3D11Device* poDevice, DxScene* poScene, ByteStream& fileData, const std::string& gameIdString)
{	// TODO: clean up -> put all asset insertion into VMDManager
	//ByteStream vmdBinary(LR"(I:\Modding\WH3\variantmeshes\variantmeshdefinitions\brt_battle_pilgrims.variantmeshdefinition)");
	//ByteStream vmdBinary(LR"(I:\Modding\WH3\variantmeshes\variantmeshdefinitions\emp_ch_karl.variantmeshdefinition)");
	//ByteStream vmdBinary(LR"(I:\Modding\WH3\variantmeshes\variantmeshdefinitions\ksl_bear_heavy_katarin.variantmeshdefinition)");
	ByteStream vmdBinary(LR"(I:\Modding\WH3\variantmeshes\variantmeshdefinitions\emp_flagellants_tattersouls.variantmeshdefinition)");

	DxVariantMeshTree variantMeshCreator;
	variantMeshCreator.Create(vmdBinary);
	variantMeshCreator.AllocateNodes();
	variantMeshCreator.GenerateVariant();

	poScene->GetRootNode()->AddChild(variantMeshCreator.GetNode());

	SetCameraAutoFit(variantMeshCreator.GetNode().get(), poScene);
}

void rldx::DxSceneCreator::AddModel(ID3D11Device* poDevice, DxScene* poScene, ByteStream& fileData, const std::string& gameIdString)
{	
	auto modelNodeRmv2 = rldx::DxNodeCreator::CreateNode<DxModelNode>("ModelNode RMV2");		

	auto newPbrShaderCreator = GameShaderProgramCreatorFactory().Get(gameIdString);
	if (!newPbrShaderCreator) {
		throw std::exception("No shader program creator found for game");
	}

	auto newPbrShaderProgram = newPbrShaderCreator->Create(poDevice, fileData);
	if (!newPbrShaderProgram) {
		throw std::exception("Error Creating Shader");
	}

	// TODO: REMOVE?
	//poScene->SetDefaultShaderProgram(newPbrShaderProgram);

	rmv2::RigidModelReader rigidModelFileReader;
	auto rmv2File = rigidModelFileReader.Read(fileData);
	modelNodeRmv2->SetModelData(poDevice, rmv2File);
	modelNodeRmv2->LoadMaterialDataFromRmv2(poDevice, rmv2File);	
	modelNodeRmv2->SetShaderProgram(newPbrShaderProgram);	
		
	poScene->GetRootNode()->AddChild(modelNodeRmv2);

	SetCameraAutoFit(modelNodeRmv2.get(), poScene);
}

void rldx::DxSceneCreator::SetCameraAutoFit(DxModelNode* modelNodeRmv2, rldx::DxScene* poScene)
{		
	auto boundBox = poScene->GetRootBoundBox();
	//auto& boundBox = modelNodeRmv2->GetNodeBoundingBox(); // get the bounding box which is the "sum" of all its mesh BB's
	const float adjustBBExtend = 0.3f;
	float bbSize = max(boundBox.Extents.z, max(boundBox.Extents.y, boundBox.Extents.x)) * (2.0f + adjustBBExtend);
	float fieldIOfView = poScene->GetCamera().GetFieldOfView();
	auto DEBUG_CENTER = boundBox.Center;

	float cameraDistance = (bbSize / 2.0f) / tan(fieldIOfView / 2.0f);

	poScene->GetCamera().SetRadius(cameraDistance);
	poScene->GetCamera().SetLookAt(boundBox.Center); // bound box fomula doesn't take "look at" into account, so move up a bit
	poScene->GetCamera().SetRotate(-DirectX::XM_PI / 4.0f, -DirectX::XM_PI / 8.0f); // rotate the scene into a neat orientation
}


class ClassToAccess
{
	friend class ClassThatAccess;
private:
	int i;
};

class ClassThatAccess
{
	void func()
	{
		ClassToAccess testClass;
		testClass.i = 1; // class can acces private members
	}
};

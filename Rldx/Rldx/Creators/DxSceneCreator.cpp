#include "DxSceneCreator.h"

#include "..\..\QtRenderingWidget\Constants\GameIdKeys.h"

#include "..\Creators\DxGameShaderCreators.h"
#include "..\Creators\DxModelNodeCreators\DxModelNodeCreator.h"
#include "..\..\..\ImportExport\WsModel\Reader\WsModelReader.h"


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
			libtools::GetExePath() + LR"(VS_Simple.cso)",
			libtools::GetExePath() + LR"(PS_Simple.cso)"
		);

	// make grid node, mesh, fill node, set shaders
	auto meshNodeGrid = rldx::DxMeshNode::Create("Grid");
	auto gridMeshData = rldx::DxMeshCreatorHelper::MakeGrid(poDevice, 40, 0.1f);

	meshNodeGrid->SetModelData(gridMeshData);
	meshNodeGrid->SetShaderProgram(newSimpleShaderProgram);
	m_newScene->GetRootNode()->AddChild(meshNodeGrid);
	

	return std::move(m_newScene);
}

void rldx::DxSceneCreator::AddModel(ID3D11Device* poDevice, DxScene* poScene, ByteStream& fileData, const std::string& gameIdString)
{	
	// TODO: REMOVE
	// BEGIN: DEBUGGIN CODE
#ifdef _DEBUG
#if 0
	ByteStream wsModelBinaryData(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\sn2\def\def_medusa\def_medusa_body_01.wsmodel)");
	auto wsModelData = rmv2::WsModelReader().Read(wsModelBinaryData);	
	// END: DEBUGGIN CODE

	auto modelNode = rldx::ModelNodeCreator().CreateNode(poDevice, poScene->GetRootNode(), fileData);
	fileData.SetOffset(0);
	
#endif	
#endif	

	auto modelNodeRmv2 = rldx::DxNodeCreator::CreateNode<DxModelNode>("ModelNode RMV2");		

	auto newPbrShaderCreator = GameShaderProgramCreatorFactory().Get(gameIdString);
	if (!newPbrShaderCreator) {
		throw std::exception("No shader program creator found for game");
	}

	auto newPbrShaderProgram = newPbrShaderCreator->Create(poDevice, fileData);
	if (!newPbrShaderProgram) {
		throw std::exception("Error Creating Shader");
	}

	poScene->SetDefaultShaderProgram(newPbrShaderProgram);

	rmv2::RigidModelReader rigidModelFileReader;
	auto rmv2File = rigidModelFileReader.Read(fileData);
	modelNodeRmv2->SetModelData(poDevice, rmv2File);
	modelNodeRmv2->LoadMaterialDataFromRmv2(poDevice, rmv2File);	
	modelNodeRmv2->SetShaderProgram(newPbrShaderProgram);

	SetCameraAutoFit(modelNodeRmv2, poScene);

	poScene->GetRootNode()->AddChild(modelNodeRmv2);
}

void rldx::DxSceneCreator::SetCameraAutoFit(std::shared_ptr<rldx::DxModelNode>& modelNodeRmv2, rldx::DxScene* poScene)
{
	auto& boundBox = modelNodeRmv2->GetBoundingBox();
	float s = boundBox.Extents.y * 2.0f;
	float a = poScene->GetCamera().GetFieldOfView();

	float d = (s / 2) / tan(a / 2);

	poScene->GetCamera().SetRadius(d);
	poScene->GetCamera().SetLookAt({ 0, 0.9f, 0 });
	poScene->GetCamera().SetRotate(-DirectX::XM_PI / 4.0f, -DirectX::XM_PI / 6.0f);
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

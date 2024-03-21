#include "DxSceneCreator.h"

#include "..\Helpers\DxMeshCreatorHelper.h"

#include "..\..\QtRenderingWidget\Constants\GameIdKeys.h"

#include "..\..\..\ImportExport\FileFormats\WsModel\Reader\WsModelReader.h"
#include "..\Creators\DxGameShaderCreators.h"

#include "..\SceneGraph\SceneNodes\DxDeformerNode.h"
#include "..\SceneGraph\SceneNodes\DxVmdNodes.h"

rldx::DxScene::Uptr rldx::DxSceneCreator::InitNewScene(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, bool isSRGB, const std::wstring& name)
{
	m_newScene = std::make_unique<DxScene>(name);

	// TODO: remove this mess? Setting the window size in win32 api?
	//SetWindowPos(m_nativeWindowHandle, nullptr, 0, 0, 2000, 1024, SWP_NOOWNERZORDER);

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

rldx::DxScene::Uptr rldx::DxSceneCreator::Create(HWND nativeWindHandle, ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::wstring& gameStringId)
{
	m_nativeWindowHandle = nativeWindHandle;

	RGBModeEnum rgbMode = RGBModeEnum::RGB_Mode; // default to RGB mode

	// These games use SRGB mode
	if (gameStringId == game_id_keys::KEY_WARHAMMER ||
		gameStringId == game_id_keys::KEY_WARHAMMER_2 ||
		gameStringId == game_id_keys::KEY_WARHAMMER_3 ||
		gameStringId == game_id_keys::KEY_PHARAOH ||
		gameStringId == game_id_keys::KEY_THREE_KINGDOMS ||
		gameStringId == game_id_keys::KEY_TROY)
	{
		rgbMode = RGBModeEnum::SRGB_Mode;
	}

	m_newScene = InitNewScene(poDevice, poDeviceContext, rgbMode);

	// -- make default, fallback shaders
	// for grid
	auto newSimpleShaderProgram =
		rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
			poDevice,
			/*libtools::GetExePath() + */LR"(VS_Simple.cso)",
			/*libtools::GetExePath() + */LR"(PS_Simple.cso)"
		);

	// TODO: I don't think this is ever used, it is needed, when there are are default matrerial? Maybe it IS needed as a fallback?
	// for no textures
	auto noTextures_ShaderProgram =
		rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
			poDevice,
			/*libtools::GetExePath() + */LR"(VS_Simple.cso)",
			/*libtools::GetExePath() + */LR"(PS_NoTextures.cso)"
		);

	AddGrid(poDevice, newSimpleShaderProgram);


	DxResourceManager::Instance()->SetDefaultShaderProgram(noTextures_ShaderProgram);



	// TODO: remove- this code from here, it goes in AddVMD

	SetCameraAutoFit(m_newScene.get());

	return std::move(m_newScene);
}

void rldx::DxSceneCreator::AddGrid(ID3D11Device* poDevice, rldx::DxMeshShaderProgram* newSimpleShaderProgram)
{
	// make grid node, mesh, fill node, set shaders
	auto meshNodeGrid = rldx::DxMeshNode::Create(L"Grid");
	auto gridMeshData = rldx::DxMeshCreatorHelper::MakeGrid(poDevice, 40, 0.1f);

	meshNodeGrid->SetMeshData(gridMeshData, L"Grid Mesh");
	meshNodeGrid->SetShaderProgram(newSimpleShaderProgram);
	m_newScene->GetSceneRootNode()->AddChild(meshNodeGrid);
	m_newScene->m_poGridNode = meshNodeGrid.get();
}

void rldx::DxSceneCreator::AddVariantMesh(ID3D11Device* poDevice, DxScene* poScene, ByteStream& fileData, const std::wstring& gameIdString)
{
	// TODO: clean block up !!!

	poScene->GetVmdManager().LoadVariantMesh(poScene->GetSceneRootNode(), fileData, gameIdString);
	//poScene->GetVmdManager().GenerateVariant();

	//// TODO: re-enable these 2 lines when done debuggin skeleteon
	//poScene->GetSceneRootNode()->AddChild(poScene->GetVmdManager().GetNode());
	/*auto skeletonNode = DxDeformerNode::Create();
	skeletonNode->LoadBindPose(LR"(animations\skeletons\humanoid01.anim)");
	skeletonNode->LoadAnimClip(LR"(K:\Modding\WH2\animations\battle\humanoid01\sword_and_shield\locomotion\hu1_sws_walk_01.anim)");

	poScene->GetSceneRootNode()->AddChild(skeletonNode);
	poScene->GetVmdManager().GetNode()->SetDeformerNodeRecursive(skeletonNode.get());*/

	//	poScene->GetSceneRootNode()->AddChild(poScene->GetVmdManager().GetNode());

	SetCameraAutoFit(poScene);
}

void rldx::DxSceneCreator::AddModel(ID3D11Device* poDevice, DxScene* poScene, ByteStream& fileData, const std::wstring& gameIdString)
{
	auto modelNodeRmv2 = rldx::DxNodeCreator::CreateNode<DxModelNode>(L"ModelNode RMV2");

	auto newPbrShaderCreator = GameShaderProgramCreatorFactory().Get(gameIdString);
	if (!newPbrShaderCreator) {
		throw std::exception("No shader program creator found for game");
	}

	auto newPbrShaderProgram = newPbrShaderCreator->Create(poDevice);
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

	poScene->GetSceneRootNode()->AddChild(modelNodeRmv2);

	SetCameraAutoFit(poScene);
}

void rldx::DxSceneCreator::SetCameraAutoFit(rldx::DxScene* poScene)
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
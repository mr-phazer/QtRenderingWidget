#include <DirectXCollision.h>

#include <CommonLibs\Logger\Logger.h>
#include "..\..\..\ImportExport\FileFormats\WsModel\Reader\WsModelReader.h"
#include "..\..\QtRenderingWidget\Constants\GameIdKeys.h"
#include "..\Creators\DxGameShaderCreators.h"
#include "..\Helpers\DxMeshCreatorHelper.h"
#include "..\SceneGraph\SceneNodes\DxDeformerNode.h"
#include "..\SceneGraph\SceneNodes\DxVmdNodes.h"
#include "DxSceneCreator.h"

namespace rldx
{

	std::unique_ptr<DxScene> DxSceneCreator::InitSceneDX(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, bool isSRGB, const std::wstring& name)
	{
		auto upoNewScene = std::make_unique<DxScene>(name);

		// TODO: remove this mess? Setting the window size in win32 api?
		//SetWindowPos(m_nativeWindowHandle, nullptr, 0, 0, 2000, 1024, SWP_NOOWNERZORDER);

		RECT windowRect;
		GetWindowRect(m_nativeWindowHandle, &windowRect);

		UINT width = windowRect.right - windowRect.left;
		UINT height = windowRect.bottom - windowRect.top;

		// create swap chain
		upoNewScene->GetRefSwapChain() = DxSwapChain::CreateForHWND(poDevice, m_nativeWindowHandle, isSRGB, width, width);
		upoNewScene->InitRenderView(poDevice);

		// TODO: enable? Shouldn't it work on its own (multi windows)
		upoNewScene->Resize(poDevice, poDeviceContext, width, width);

		// Create a separate "assset node", so assets can be deleted indepdently of grid
		auto asssetNode = DxBaseNode::Create(L"Asset Node");
		upoNewScene->GetSceneRootNode()->AddChild(asssetNode);
		upoNewScene->m_poAssetNode = asssetNode.get();

		return std::move(upoNewScene);
	}

	std::unique_ptr<DxScene> DxSceneCreator::Create(HWND nativeWindHandle, ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::wstring& gameStringId)
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

		m_upoNewScene = InitSceneDX(poDevice, poDeviceContext, rgbMode);

		// -- make default, fallback shaders
		// for grid
		auto newSimpleShaderProgram =
			DxMeshShaderProgram::CreateFromDisk<DxMeshShaderProgram>(
				poDevice,
				LR"(VS_Simple.cso)",
				LR"(PS_Simple.cso)"
			);

		// TODO: I don't think this is ever used, it is needed, when there are are default matrerial? Maybe it IS needed as a fallback?
		// for no textures
		auto noTextures_ShaderProgram =
			DxMeshShaderProgram::CreateFromDisk<DxMeshShaderProgram>(
				poDevice,
				LR"(VS_Simple.cso)",
				LR"(PS_NoTextures.cso)"
			);

		AddGrid(poDevice, newSimpleShaderProgram.GetPtr());

		SetCameraAutoFit(m_upoNewScene.get());

		return std::move(m_upoNewScene);
	}

	void DxSceneCreator::AddGrid(ID3D11Device* poDevice, DxMeshShaderProgram* newSimpleShaderProgram)
	{
		// make grid node, mesh, fill node, set shaders
		auto meshNodeGrid = DxMeshNode::Create(L"Grid");
		auto gridMeshData = DxMeshCreatorHelper::MakeGrid(poDevice, 40, 0.1f);

		meshNodeGrid->SetMeshData(gridMeshData, L"Grid Mesh");
		meshNodeGrid->SetShaderProgram(newSimpleShaderProgram);

		m_upoNewScene->GetSceneRootNode()->AddChild(meshNodeGrid);
		m_upoNewScene->m_poGridNode = meshNodeGrid.get();
	}

	void DxSceneCreator::AddVariantMesh(ID3D11Device* poDevice, DxScene* poScene, ByteStream& fileData, const std::wstring& gameIdString)
	{
		logging::LogAction(L"Start Clearing Existing Assets");
		// remove any existing asset
		poScene->GetAssetNode()->RemoveChildren();

		logging::LogAction(L"Done Clearing Existing Assets");
		poScene->GetVmdManager().LoadVariantMesh(poScene->GetAssetNode(), fileData, gameIdString);
		poScene->GetVmdManager().GetNewVariant();
		poScene->GetSceneRootNode()->UpdateAllBoundBoxes();

		SetCameraAutoFit(poScene);
	}

	// TODO: remove? is this needed anymore, when "addvariantmesh" takes care of all cases??
	void DxSceneCreator::AddModel(ID3D11Device* poDevice, DxScene* poScene, ByteStream& fileData, const std::wstring& gameIdString)
	{
		auto modelNodeRmv2 = DxNodeCreator::CreateNode<DxModelNode>(L"ModelNode RMV2");

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

	void DxSceneCreator::SetCameraAutoFit(DxScene* poScene)
	{
		auto boundBox = poScene->GetRootBoundBox();
		//auto& boundBox = modelNodeRmv2->GetNodeBoundingBox(); // get the bounding box which is the "sum" of all its mesh BB's
		//const float adjustBBExtend = 0.3f;
		float bbSize = max(boundBox.Extents.z, max(boundBox.Extents.y, boundBox.Extents.x))/* * (2.0f + adjustBBExtend)*/;
		float fieldIOfView = poScene->GetCamera().GetFieldOfView();
		auto DEBUG_CENTER = boundBox.Center;
		float cameraDistance = (bbSize / 2.0f) / tan(fieldIOfView / 2.0f);

		// Get the radius of a sphere that encapsulates the bounding box
		float radius = bbSize;

		// Calculate the horizontal and vertical field of view
		float halfVerticalFOV = fieldIOfView / 2.0f;
		float halfHorizontalFOV = atan(tan(halfVerticalFOV) * poScene->GetCamera().GetAspectRatio());

		// Calculate distance based on the larger field of view to ensure everything is visible
		float distanceVertical = radius / sin(halfVerticalFOV);
		float distanceHorizontal = radius / sin(halfHorizontalFOV);

		//poScene->GetCamera().SetRadius(std::max<float>(distanceVertical, distanceHorizontal));
		poScene->GetCamera().SetEyePosition(sm::Vector3(0, 0, std::max<float>(distanceVertical, distanceHorizontal)));
		poScene->GetCamera().SetLookAt(boundBox.Center); // bound box fomula doesn't take "look at" into account, so move up a bit
		poScene->GetCamera().SetRotate(-DirectX::XM_PI / 4.0f, -DirectX::XM_PI / 8.0f); // rotate the scene into a neat orientation
	}
}
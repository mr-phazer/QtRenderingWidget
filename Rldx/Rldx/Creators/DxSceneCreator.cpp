#include <DirectXCollision.h>

#include <CommonLibs\Logger\Logger.h>
#include <CustomExceptions\CustomExceptions.h>
#include "..\..\..\ImportExport\FileFormats\WsModel\Reader\WsModelReader.h"
#include "..\..\QtRenderingWidget\Constants\GameIdKeys.h"
#include "..\Creators\DxGameShaderCreators.h"
#include "..\Helpers\DxMeshCreatorHelper.h"
#include "..\Rendering\DxMesh.h"
#include "..\SceneGraph\SceneNodes\DxDeformerNode.h"
#include "..\SceneGraph\SceneNodes\DxVmdNodes.h"
#include "DxSceneCreator.h"

using namespace utils;

namespace rldx
{
	std::unique_ptr<DxScene> DxSceneCreator::CreateScene(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, bool isSRGB, const std::wstring& m_nodeName)
	{
		RECT windowRect;
		GetClientRect(m_nativeWindowHandle, &windowRect); // get only the client area

		UINT width = windowRect.right - windowRect.left;
		UINT height = windowRect.bottom - windowRect.top;

		// create swap chain
		auto newSwapChain = DxSwapChain::CreateForHWND(poDevice, poDeviceContext, m_nativeWindowHandle, isSRGB, width, height);

		// create scene
		auto upoNewScene = std::make_unique<DxScene>(m_nodeName, std::move(newSwapChain));

		// Create a separate "assset node", so assets can be deleted indepdently of grid
		auto asssetNode = DxBaseNode::Create(L"Asset Node");
		upoNewScene->m_poAssetNode = &upoNewScene->GetSceneRootNode()->AddChild(std::move(asssetNode));

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

		m_upoNewScene = CreateScene(poDevice, poDeviceContext, rgbMode);

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
		using namespace DirectX;
		// make grid node, mesh, fill node, set shaders
		auto meshNodeGrid = std::make_unique<DxMeshNode>(L"Grid");
		auto gridMeshData = DxMeshCreatorHelper::MakeGrid(poDevice, 40, 0.1f);

		meshNodeGrid->SetMeshData(gridMeshData, L"Grid Mesh");
		meshNodeGrid->SetShaderProgram(newSimpleShaderProgram);

		// TODO: change? sets the grid to be of no size, so it doesn't interfere with the camera
		meshNodeGrid->NodeBoundingBox() = DirectX::BoundingBox({ 0,0,0 }, { 0E-7, 0E-7, 0E-7 });

		m_upoNewScene->m_poGridNode = meshNodeGrid.get();

		m_upoNewScene->GetSceneRootNode()->AddChild(std::move(meshNodeGrid));
	}

	void DxSceneCreator::AddVariantMesh(ID3D11Device* poDevice, DxScene* poScene, ByteStream& fileData, const std::wstring& gameIdString)
	{
		poScene->GetSceneRootNode()->NodeBoundingBox() = DirectX::BoundingBox({ 0,0,0 }, { 0E-7, 0E-7, 0E-7 });
		poScene->GetAssetNode()->NodeBoundingBox() = DirectX::BoundingBox({ 0,0,0 }, { 0E-7, 0E-7, 0E-7 });
		poScene->GetVmdManager().LoadVariantMeshIntoNode(poScene->GetAssetNode(), fileData, gameIdString);
		poScene->GetVmdManager().GenerateNewVariant();



#ifdef _DEBUG
		DxBaseNode::AllocateBoundingBoxMeshesRecursive(poScene->GetSceneRootNode());
#endif

		// TODO: FIX issue: the largest loaded model leaves in a "top" bound volume
		// so, after loading a big model, the camera never zooms in again, FIXXX
		SetCameraAutoFit(poScene);
	}

	// TODO: remove? is this needed anymore, when "addvariantmesh" takes care of all cases??
	void DxSceneCreator::AddModel(ID3D11Device* poDevice, DxScene* poScene, ByteStream& fileData, const std::wstring& gameIdString)
	{
		// TODO: REMOVE this line if everything works
		//auto modelNodeRmv2 = DxNodeCreator::CreateNode<DxModelNode>(L"ModelNode RMV2");
		auto modelNodeRmv2 = std::make_unique<DxModelNode>(L"ModelNode RMV2");

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

		// TODO:: create in place and edit now-owning pointer
		poScene->GetSceneRootNode()->AddChild(std::move(modelNodeRmv2));

		SetCameraAutoFit(poScene);
	}

	void DxSceneCreator::SetCameraAutoFit(DxScene* poScene)
	{
		auto boundBox = poScene->GetRootBoundBox();

		float bbSize = max(boundBox.Extents.z, max(boundBox.Extents.y, boundBox.Extents.x))/* * (2.0f + adjustBBExtend)*/;

		if (bbSize < 0.000001f) // if nothing or something completely flat is in view set default zoom
		{
			bbSize = 2.0f;
		}

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

		// TODO: renaable
		poScene->GetCamera().SetRotate(-DirectX::XM_PI / 4.0f, -DirectX::XM_PI / 8.0f); // rotate the scene into a neat orientation
	}
}
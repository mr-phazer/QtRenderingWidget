#include "DxScene.h"

#include "..\..\..\DXUT\Core\DXUTmisc.h"
#include "..\..\ImportExport\FileFormats\RigidModel\Readers\RigidModelReader.h"
#include "..\Creators\DxMeshCreator.h"
#include "..\Creators\DxMeshCreator.h"
#include "..\Managers\DxDeviceManager.h"
#include "..\SceneGraph\Helpers\SceneGraphParser.h"
#include "..\SceneGraph\SceneNodes\DxModelNode.h"
#include "DxMeshRenderBucket.h"


using namespace rldx;
using namespace utils;

void DxScene::Draw(ID3D11DeviceContext* poDeviceContext)
{
	// -- set target, and clear pixels and depth buffer
	m_spoSwapChain->GetBackBuffer()->BindAsRenderTargetViewWithDepthBuffer(poDeviceContext);

	// TODO: set color conditional on "game ID", as older games in RGB, newer in sRGB, so the BG color will look very different
	m_spoSwapChain->GetBackBuffer()->ClearPixelsAndDepthBuffer(poDeviceContext, { 0.1f, 0.1f, 0.1f, 1.0f });

	m_spoSwapChain->UpdateViewPort(poDeviceContext, nullptr);

	//  fetch mesh nodes from scenegraph
	m_sceneGraph.FillRenderBucket(&m_renderQueue);

	// -- update + set scene (per frame) constant buffer	
	BindToDC(poDeviceContext);

	m_renderQueue.Draw(poDeviceContext, m_poDefaultShaderProgram);

	// diable depth buffer, while drawing 2d text
// TODO: do NOT create this in the render 
	poDeviceContext->OMSetDepthStencilState(m_upoCommonStates->DepthNone(), 0);

	DxDeviceManager::GetInstance().GetDebugTextWriter()->RenderText();

	// re-enable depthbuffer
	// TODO: do NOT create states in render loop, store them
	poDeviceContext->OMSetDepthStencilState(m_upoCommonStates->DepthDefault(), 0);

	// TODO: do NOT create this in the render loop
	poDeviceContext->RSSetState(m_upoCommonStates->CullNone());


	m_spoSwapChain->Present(poDeviceContext);
}

DxBaseNode* DxScene::GetSceneRootNode() const
{
	return m_sceneGraph.GetRootNode();
}

void rldx::DxScene::SetGridState(DxBaseNode::DrawStateEnum drawState)
{
	if (!m_poGridNode) return;

	m_poGridNode->SetDrawState(drawState);
}

DxMeshNode* rldx::DxScene::GetGridNode() const
{
	return m_poGridNode;
}

DxBaseNode* rldx::DxScene::GetAssetNode() const
{
	return m_poAssetNode;
}

// TODO: test this
void DxScene::DeleteNode(DxBaseNode* node)
{
	auto nodeResult = DxBaseNode::FindChild(node, GetSceneRootNode());

	if (!nodeResult) {
		return;
	};

	if (nodeResult->GetParent()) {
		return;
	}

	nodeResult->GetParent()->RemoveChild(nodeResult);
}

void DxScene::ClearRenderQueue()
{
	m_renderQueue.ClearItems();
}

//void DxScene::MakeSceneSwapChain(ID3D11Device* poDevice, HWND nativeWindowHandle)
//{
//	SetWindowPos(m_hwndNativeWindowHandle, nullptr,  0, 0, 1024, 1024, SWP_NOOWNERZORDER);
//	
//	RECT windowRect;
//	GetWindowRect(m_hwndNativeWindowHandle = nativeWindowHandle, &windowRect);
//
//	UINT width = windowRect.right - windowRect.left;
//	UINT height = windowRect.bottom - windowRect.top;
//
//	// create swap chain
//	GetRefSwapChain() = DxSwapChain::CreateForHWND(poDevice, m_hwndNativeWindowHandle, , width, height);	
//}

LRESULT __stdcall DxScene::ForwardNativeWindowEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_CONTROL)
			m_bCtrlDown = true;
	}

	if (uMsg == WM_KEYUP)
	{
		if (wParam == VK_CONTROL)
			m_bCtrlDown = false;
	}

	if (m_bCtrlDown) // send messages to light controller "camera"
	{
		return m_globalDirectionalLight.HandleMessages(hWnd, uMsg, wParam, lParam);
	}

	return m_globalCamera.HandleMessages(hWnd, uMsg, wParam, lParam);
}

void DxScene::Update(float timeElapsed)
{
	UpdateViewAndPerspective();

	m_globalCamera.MoveFrame(timeElapsed);
	m_globalDirectionalLight.MoveFrame(timeElapsed);

	m_sceneGraph.UpdateNodes(timeElapsed);
}

// TODO: remove? Or enabled setting types/names in another constructor
//DxScene::DxScene()
//{
//	SetType(DxSceneTypeEnum::Normal);
//	SetTypeString(L"Resource:DxScene");
//}

DxScene::~DxScene() {
	// TODO: REMOVE DEBUGGIN CODE
	auto DEBUG_dxScene_constructor_break = 1;

	m_renderQueue.ClearItems();




	// TODO: make sure all these are deleted
	/*

	DxSceneGraph m_sceneGraph;
	DxMeshRenderBucket m_renderQueue;
	DxSwapChain::Uptr m_spoSwapChain; // back buffer
	DxAmbientLightSource m_ambientLightSource;
	DxTextureSamplers m_textureSamplers;

	std::unique_ptr<DirectX::CommonStates> m_upoCommonStates;
	*/
}

DxScene::DxScene(rldx::DxResourceManager& m_resourceManager, const std::wstring& name, std::unique_ptr<DxSwapChain> upoSwapChain)
	:
	m_resourceManager(m_resourceManager),
	m_vmdManager(m_resourceManager)
{
	DxDeviceManager::GetInstance().GetDebugTextWriter()->AddString(L"QtRenderingWidget for RPFM version 0.0.1a.", { 1,1,1,1 }, 6.0f);
	SetName(name);

	// TODO: move more of the initializing into the constructor, RAII
	m_spoSwapChain = std::move(upoSwapChain);

	InitScene(DxDeviceManager::Device());

	// TODO: enable? Shouldn't it work on its own (multi windows)
	Resize(DxDeviceManager::Device(), DxDeviceManager::DeviceContext(), m_spoSwapChain->GetBackBuffer()->GetWidth(), m_spoSwapChain->GetBackBuffer()->GetHeight());
}

void DxScene::InitScene(ID3D11Device* poDevice)
{
	m_globalCamera.SetProjParams(DirectX::XM_PI / 4, m_spoSwapChain->GetBackBuffer()->GetAspectRatio(), 0.01f, 100.0f);;
	m_globalDirectionalLight.SetRotationScale(0.005f);
	m_globalDirectionalLight.SetRotate(5.48f, 5.95f);

	m_upoCommonStates = std::make_unique<DirectX::CommonStates>(poDevice);

	m_sceneFrameVSConstBuffer.Init(poDevice, "VS_PerFrame_CB");
	m_sceneFramePSConstBuffer.Init(poDevice, "PS_PerFrame_CB");

	DXUT_SetDebugName(m_sceneFramePSConstBuffer.GetBuffer(), "PS_CB:DirectionalLight");

	// TODO: test? Needed?
	std::wstring cubeMapFolder = L"Textures/CubeMaps/"; // std::wstring(DxResourceManager::GetDefaultAssetFolder());// +LR"(\Textures\CubeMaps\)";

	// TODO: Put into DxAmbientLightSource::CreateFromFiles()
	ByteStream iblDiffuseMapBinary(cubeMapFolder + L"LandscapeCubeMapIBLDiffuse.dds");
	ByteStream iblSPecularMapBinary(cubeMapFolder + L"LandscapeCubeMapIBLSpecular.dds");
	ByteStream iblLUTBinary;

	m_ambientLightSource =
		DxAmbientLightSource::Create(
			poDevice,
			m_resourceManager,
			iblDiffuseMapBinary,
			iblSPecularMapBinary,
			iblLUTBinary
		);

	m_ambientLightSource.SetLightRadiance(0.2f);
	m_textureSamplers = DxTextureSamplers::Create(*m_upoCommonStates);
}

void DxScene::Resize(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, unsigned int width, unsigned int height)
{
	m_spoSwapChain->Resize(poDevice, poDeviceContext, width, height);
	m_globalCamera.SetWindow(width, height);
}

void DxScene::StoreNode(DxBaseNode* node)
{
	m_sceneGraph.AddNodeToLinearIndexTable(node);
}

void DxScene::UpdateViewAndPerspective()
{
	m_sceneFrameVSConstBuffer.data.view = m_globalCamera.GetViewMatrix().Transpose();
	m_sceneFrameVSConstBuffer.data.projection = m_globalCamera.GetProjMatrix().Transpose();
	m_sceneFrameVSConstBuffer.data.eyePosition = m_globalCamera.GetEyePt();

	// TODO: set a proper direction as init:
	m_globalDirectionalLight.GetViewMatrix();
	m_sceneFramePSConstBuffer.data.lightData[0].direction = -m_globalDirectionalLight.GetEyePt(); // negative = light comes from "forward"
	m_sceneFramePSConstBuffer.data.lightData[0].radiance = m_globalDirectionalLight.GetRadius();
}

void DxScene::BindToDC(ID3D11DeviceContext* poDeviceContext)
{
	m_sceneFrameVSConstBuffer.SetStartSlot(0);
	m_sceneFrameVSConstBuffer.RefreshGPUData(poDeviceContext);


	m_ambientLightSource.BindToDC(poDeviceContext);

	m_sceneFramePSConstBuffer.SetStartSlot(1);
	m_sceneFramePSConstBuffer.RefreshGPUData(poDeviceContext);


	//m_ambientLightSource.m_oPSConstBuffer.RefreshGPUData(poDeviceContext);
	//m_ambientLightSource.m_oPSConstBuffer.SetStartSlot(0);



	// vertex	//  shader buffer
	ID3D11Buffer* vertexShaderSceneConstBuffers[1] = { m_sceneFrameVSConstBuffer.GetBuffer() };
	poDeviceContext->VSSetConstantBuffers(0, 1, vertexShaderSceneConstBuffers);

	// TODO: build this into "ConstBuffer"??
	// pixel shader const buffer
	//ID3D11Buffer* pPS_AmbientLIghtBuffer[] = 
	//{ 
	//	m_ambientLightSource.m_oPSConstBuffer.GetBuffer(), 		
	//};
	//poDeviceContext->PSSetConstantBuffers(0, 1, pPS_AmbientLIghtBuffer);

	// TODO: build this into "ConstBuffer"??
	// vertex shader const buffer
	ID3D11Buffer* m_pPS_DirectionalLightBuffer[] =
	{
		m_sceneFramePSConstBuffer.GetBuffer(),
	};
	poDeviceContext->PSSetConstantBuffers(1, 1, m_pPS_DirectionalLightBuffer);

	m_textureSamplers.BindToDC(poDeviceContext);
}

void DxScene::DEBUGGING_SetViewAndPerspective()
{
	// TODO: method full of crap, is it still useful/remove?
	// Use DirectXMath to create view and perspective matrices, for debuggin purposes
	DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.7f, 1.5f, 0.f);
	DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, -0.1f, 0.0f, 0.f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.f);

	DirectX::XMStoreFloat4x4(
		&m_sceneFrameVSConstBuffer.data.view,
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixLookAtRH(
				eye,
				at,
				up
			)
		)
	);

	m_sceneFrameVSConstBuffer.data.view = sm::Matrix::Identity;

	float aspectRatioX = m_spoSwapChain->GetBackBuffer()->GetAspectRatio();
	float aspectRatioY = aspectRatioX < (16.0f / 9.0f) ? aspectRatioX / (16.0f / 9.0f) : 1.0f;

	DirectX::XMStoreFloat4x4(
		&m_sceneFrameVSConstBuffer.data.projection,
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixPerspectiveFovRH(
				2.0f * std::atan(std::tan(DirectX::XMConvertToRadians(70) * 0.5f) / aspectRatioY),
				aspectRatioX,
				0.01f,
				100.0f
			)
		)
	);
}




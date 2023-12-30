
#include "DxScene.h"
#include "..\Helpers\DxMeshCreator.h"
#include "..\SceneGraph\Helpers\SceneGraphParser.h"

using namespace Rldx;

// TODO: remove? All this happens in the SceneCreators
//void Rldx::DxScene::Init(ID3D11Device* poDevice, HWND nativeWindowHandle)
//{
//	// TODO: make DxScene a base class, and only add stuf to "init" in the derived "DxScene"s?
//	m_vertexShaderConstantBuffer.Create(poDevice);
//
//	// create swap chain
//	auto meshNode = DxMeshNode::Create("MeshNode1");
//
//	auto testMeshCube = MakeTestCubeMesh(poDevice);
//	meshNode->SetMeshData(testMeshCube);
//
//	m_spoRootNode->AddChild(meshNode);
//
//	// TODO: remove;
//	auto debug_1 = 1;
//}

//std::string Rldx::DxScene::GetTypeString() const { return "DxScene"; }

void Rldx::DxScene::Draw(ID3D11DeviceContext* poDeviceContext)
{
	auto rasterizerStateNoCull = m_upoCommonStates->CullNone();
	poDeviceContext->RSSetState(rasterizerStateNoCull);

	// -- set target, and clear pixels and depth buffer
	m_spoSwapChain->GetBackBuffer()->BindAsRenderTargetViewWithDepthBuffer(poDeviceContext);
	m_spoSwapChain->GetBackBuffer()->ClearPixelsAndDepthBuffer(poDeviceContext, { 0.1, 0.1f, 0.1, 1 });
	m_spoSwapChain->UpdateViewPort(poDeviceContext, nullptr);


	m_sceneGraph.FetchNodes(GetRootNode(), &m_renderQueue);

	// -- update + set scene (per frame) constant buffer	
	UpdateAndBindVSConstBuffer();

	m_renderQueue.Draw(poDeviceContext);

	m_spoSwapChain->Present(poDeviceContext);
}


DxBaseNode* Rldx::DxScene::GetRootNode()
{
	return m_sceneGraph.GetRootNode();
}

// TODO: test this
void Rldx::DxScene::DeleteNode(DxBaseNode* node)
{
	auto nodeResult = DxBaseNode::FindChild(node, GetRootNode());

	if (nodeResult != nullptr)
	{
		if (nodeResult->GetParent())
		{
			nodeResult->GetParent()->RemoveChild(nodeResult);
		}
	}
}

static bool bCtrlDown = false;
LRESULT __stdcall Rldx::DxScene::NativeWindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN && wParam == 'A')
	{
		bCtrlDown = true;
	}

	if (uMsg == WM_KEYUP && wParam == 'A')
	{
		bCtrlDown = false;
	}

	if (bCtrlDown)
	{
		return m_globalLighting.HandleMessages(hWnd, uMsg, wParam, lParam);
	}

	auto ret = m_globalCamera.HandleMessages(hWnd, uMsg, wParam, lParam);

	return ret;
}

void Rldx::DxScene::Update(float timeElapsed)
{	
	UpdateViewAndPerspective();
	m_sceneGraph.UpdateNodes(GetRootNode(), timeElapsed);

	m_globalCamera.MoveFrame(timeElapsed);
	m_globalLighting.MoveFrame(timeElapsed);
}

void Rldx::DxScene::Init(ID3D11Device* poDevice)
{
	m_globalCamera.SetProjParams(DirectX::XM_PI / 4, m_spoSwapChain->GetBackBuffer()->GetAspectRatio(), 0.01f, 100.0f);;

	m_upoCommonStates = make_unique<DirectX::CommonStates>(poDevice);

	m_sceneFrameVSConstBuffer.buffer.Create(poDevice);
	m_sceneFramePSConstBuffer.buffer.Create(poDevice);
}

void Rldx::DxScene::Resize(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, unsigned int width, unsigned int height)
{
	m_spoSwapChain->Resize(poDevice, poDeviceContext, width, height);
	m_globalCamera.SetWindow(width, height);
}

void Rldx::DxScene::UpdateViewAndPerspective()
{
	m_sceneFrameVSConstBuffer.data.view = m_globalCamera.GetViewMatrix().Transpose();
	m_sceneFrameVSConstBuffer.data.projection = m_globalCamera.GetProjMatrix().Transpose();
	m_sceneFrameVSConstBuffer.data.eyePosition = m_globalCamera.GetEyePt();

	m_globalLighting.GetViewMatrix();
	m_sceneFramePSConstBuffer.data.direction = m_globalLighting.GetEyePt();

	// TODO: DEBUG: "Sub in MS Demo "rendering framework" projectiong and view matrix, and see which one is wrong"
	// _DEBUGGING_SetViewAndPerspective();
}

inline void Rldx::DxScene::UpdateAndBindVSConstBuffer()
{
	auto poDeviceContext = DxDeviceManager::DeviceContext();

	//m_sceneFrameVSConstBuffer.buffer.SetData(poDeviceContext, m_sceneFrameVSConstBuffer.data);
	m_sceneFrameVSConstBuffer.CopyDataToGPU(poDeviceContext);
	m_sceneFramePSConstBuffer.CopyDataToGPU(poDeviceContext);
	
	ID3D11Buffer* vertexShaderSceneConstBuffers[1] = { m_sceneFrameVSConstBuffer.buffer.GetBuffer() };
	poDeviceContext->VSSetConstantBuffers(0, 1, vertexShaderSceneConstBuffers);
	
	ID3D11Buffer* pixelShaderSceneConstBuffers[1] = { m_sceneFramePSConstBuffer.buffer.GetBuffer() };
	poDeviceContext->PSSetConstantBuffers(0, 1, pixelShaderSceneConstBuffers);
}

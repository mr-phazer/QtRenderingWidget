

#include "DxScene.h"
#include "..\Managers\DxDeviceManager.h"

#include "..\Helpers\DxMeshCreator.h"
#include "..\SceneGraph\Helpers\SceneGraphParser.h"

using namespace Rldx;

void Rldx::DxScene::Draw(ID3D11DeviceContext* poDeviceContext)
{
	
	// -- set target, and clear pixels and depth buffer
	m_spoSwapChain->GetBackBuffer()->BindAsRenderTargetViewWithDepthBuffer(poDeviceContext);
	m_spoSwapChain->GetBackBuffer()->ClearPixelsAndDepthBuffer(poDeviceContext, { 0.1, 0.1f, 0.1, 1 });
	m_spoSwapChain->UpdateViewPort(poDeviceContext, nullptr);
	

	poDeviceContext->OMSetDepthStencilState(m_upoCommonStates->DepthNone(), 0);

	auto textWriter = DxDeviceManager::GetInstance().GetDebugTextWriter();
	textWriter->AddString(L"QtRenderingViewWidget for RPFM version 0.0.1a.");
	textWriter->RenderText();

	poDeviceContext->OMSetDepthStencilState(m_upoCommonStates->DepthDefault(), 0);

	auto rasterizerStateNoCull = m_upoCommonStates->CullNone();
	poDeviceContext->RSSetState(rasterizerStateNoCull);

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
	if (uMsg == WM_KEYDOWN)
	{ 
		bCtrlDown = (wParam == VK_CONTROL);
		auto debug_1 = 1;
	}	

	if (uMsg == WM_KEYUP)
	{ 
		bCtrlDown = !(wParam == VK_CONTROL);
		auto debug_1 = 1;
	}	

	if (bCtrlDown)
	{
		return m_globalDirectionalLight.HandleMessages(hWnd, uMsg, wParam, lParam);
	}
	
	return m_globalCamera.HandleMessages(hWnd, uMsg, wParam, lParam);	
}

void Rldx::DxScene::Update(float timeElapsed)
{	
	UpdateViewAndPerspective();
	m_sceneGraph.UpdateNodes(GetRootNode(), timeElapsed);

	m_globalCamera.MoveFrame(timeElapsed);
	m_globalDirectionalLight.MoveFrame(timeElapsed);
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

	m_globalDirectionalLight.GetViewMatrix();
	m_sceneFramePSConstBuffer.data.direction = m_globalDirectionalLight.GetEyePt();

	// TODO: DEBUG: "Sub in MS Demo "rendering framework" projectiong and view matrix, and see which one is wrong"
	// _DEBUGGING_SetViewAndPerspective();
}

inline void Rldx::DxScene::UpdateAndBindVSConstBuffer()
{
	auto poDeviceContext = DxDeviceManager::DeviceContext();

	//m_sceneFrameVSConstBuffer.buffer.SetData(poDeviceContext, m_sceneFrameVSConstBuffer.data);
	m_sceneFrameVSConstBuffer.RefreshGPUData(poDeviceContext);
	m_sceneFramePSConstBuffer.RefreshGPUData(poDeviceContext);
	
	ID3D11Buffer* vertexShaderSceneConstBuffers[1] = { m_sceneFrameVSConstBuffer.buffer.GetBuffer() };
	poDeviceContext->VSSetConstantBuffers(0, 1, vertexShaderSceneConstBuffers);
	
	ID3D11Buffer* pixelShaderSceneConstBuffers[1] = { m_sceneFramePSConstBuffer.buffer.GetBuffer() };
	poDeviceContext->PSSetConstantBuffers(0, 1, pixelShaderSceneConstBuffers);
}

inline void Rldx::DxScene::DEBUGGING_SetViewAndPerspective()
{
	// Use DirectXMath to create view and perspective matrices.

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

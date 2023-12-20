
#include "DxScene.h"
#include "..\Helpers\DxMeshCreator.h"

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
	

	m_spoSwapChain->GetBackBuffer()->BindAsRenderTargetViewWithDepthBuffer(poDeviceContext);
	// clear back buffer
	m_spoSwapChain->GetBackBuffer()->ClearPixelsAndDepthBuffer(poDeviceContext, { 0.1, 0.1f, 0.1, 1 });

	// set back buffer to active render target
	

	// update + set scene constant buffer
	
	UpdateVSConstBuffer(poDeviceContext);

	// parse graph and draw
	// TODO: remove, this is just a stand-in for a scene-graph parser
	auto poChild = GetRootNode()->GetChild();
	auto poMeshNode = dynamic_cast<DxMeshNode*>(poChild);
	if (poMeshNode)
	{
		poMeshNode->Draw(poDeviceContext);
	}


	/*
	- first test, draw one simple mesh.


	- process scene graph to get all meshnodes
	- put mesh nodes in a container "class DxRenderQueue"
	- for each mesh node in DxRenderQueue
	- user DxRenderPass::Draw(DxRenderQueue queueToDraw)




	*/

	m_spoSwapChain->Present(poDeviceContext);
}

IDxSceneNode* Rldx::DxScene::GetRootNode() 
{ 
	return m_spoRootNode.get(); 
}

// TODO: test this
void Rldx::DxScene::DeleteNode(IDxSceneNode* node)
{
	auto nodeResult = IDxSceneNode::FindChild(node, GetRootNode());

	if (nodeResult != nullptr)
	{
		if (nodeResult->GetParent())
		{
			nodeResult->GetParent()->RemoveChild(nodeResult);
		}
	}
}

void Rldx::DxScene::DoFrameMovement(float elapsedTime)
{
	m_globalCamera.FrameMove(elapsedTime);
	UpdateViewAndPerspective();
}

void Rldx::DxScene::AllocVertexShaderConstBuffer(ID3D11Device* poDevice)
{
	m_upoCommonStates = std::unique_ptr<DirectX::CommonStates>(new DirectX::CommonStates(poDevice));

	m_vertexShaderConstantBuffer.Create(poDevice);
}



#include "DxScene.h"

using namespace Rldx;

void Rldx::DxScene::Init(ID3D11Device* poDevice, HWND nativeWindowHandle)
{
	m_vertexShaderConstantBuffer.Create(poDevice);

	// create swap chain
	auto meshNode = DxMeshNode::Create("MeshNode1");
	m_spoRootNode->AddChild(meshNode);


	auto debug_1 = 1;

}

inline DxSceneNode* Rldx::DxScene::GetRootNode() { return m_spoRootNode.get(); }

// TODO: test this
void Rldx::DxScene::DeleteNode(DxSceneNode* node)
{
	auto nodeResult = DxSceneNode::FindChild(node, GetRootNode());

	if (nodeResult != nullptr)
	{
		if (nodeResult->GetParent())
		{
			nodeResult->GetParent()->RemoveChild(nodeResult);
		}
	}
}


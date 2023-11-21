#include "DxScene.h"

using namespace Rldx;

inline DxSceneNode* Rldx::DxScene::GetRootNode() { return m_spoRootNode.get(); }

// TODO: test this
void Rldx::DxScene::DeleteNode(DxSceneNode* node)
{
	auto nodeResult = DxSceneNode::FindNode(node, GetRootNode());

	if (nodeResult != nullptr)
	{
		if (nodeResult->GetParent())
		{
			nodeResult->GetParent()->RemoveChild(nodeResult);
		}
	}
}


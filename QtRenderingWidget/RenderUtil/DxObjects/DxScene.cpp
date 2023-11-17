#include "DxScene.h"

using namespace Rldx;

inline SceneNode* Rldx::DxScene::GetRootNode() { return m_spoRootNode.get(); }

// TODO: test this
void Rldx::DxScene::DeleteNode(SceneNode* node)
{
	auto nodeResult = SceneNode::FindNode(node, GetRootNode());

	if (nodeResult != nullptr)
	{
		if (nodeResult->GetParent())
		{
			nodeResult->GetParent()->RemoveChild(nodeResult);
		}
	}
}


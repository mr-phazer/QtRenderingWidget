#include "DxSceneManager.h"

using namespace Rldx;

// TODO: test this
static SceneNode* SearchForNode(SceneNode* nodeToFind, SceneNode* currentNode)
{
	if (currentNode == nodeToFind) {
		return currentNode;
	}

	for (auto child : currentNode->GetChildren())
	{
		auto result = SearchForNode(nodeToFind, child.get());

		if (result != nullptr)
			return result;
	}

	return nullptr;
}

SceneNode* Rldx::DxSceneManager::GetRootNode()
{
	return m_rppp
}

// TODO: test this
void Rldx::DxSceneManager::DeleteNode(SceneNode* node)
{
	auto nodeResult = SearchForNode (node, GetRootNode());

	if (nodeResult != nullptr)
	{
		if (nodeResult->GetParent())
		{
			nodeResult->GetParent()->RemoveChild(nodeResult);
		}		
	}
}

#include "SceneGraphParser.h"

bool Rldx::SceneGraphParser::FindMeshNodes(IDxSceneNode* pRootNode)
{
	m_meshNodes.clear();
	return FindMeshNodesRecursive(pRootNode);
}

std::vector<Rldx::DxMeshNode*> Rldx::SceneGraphParser::GetResult()
{
	return m_meshNodes;
}

bool Rldx::SceneGraphParser::FindMeshNodesRecursive(IDxSceneNode* pRootNode)
{
	bool bError = false;

	if (pRootNode->GetType() == SceneNodeTypeEnum::Mesh)
	{
		DxMeshNode* pMeshNode = static_cast<DxMeshNode*>(pRootNode);
		m_meshNodes.push_back(pMeshNode);
	}

	for (auto& child : pRootNode->GetChildren())
	{
		FindMeshNodesRecursive(child.get());
	}

	return true;
}

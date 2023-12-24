#include "SceneGraphParser.h"

bool Rldx::SceneGraphParser::FindMeshNodes(IDxSceneNode* pRootNode)
{
	bool bError = false;

	if (pRootNode->GetType() == SceneNodeTypeEnum::Mesh)
	{
		DxMeshNode* pMeshNode = static_cast<DxMeshNode*>(pRootNode);
		m_meshNodes.push_back(pMeshNode);
	}

	for (auto& child : pRootNode->GetChildren())
	{
		FindMeshNodes(child.get());
	}

	return true;
}

std::vector<Rldx::DxMeshNode*> Rldx::SceneGraphParser::GetResult() 
{ 
	return m_meshNodes; 
}

//#include "SceneGraphParser.h"
//
//#include "..\..\Managers\DxDeviceManager.h"
//
//using namespace rldx;
//
//bool rldx::SceneGraphParser::FindMeshNodes(IDxSceneNode* pRootNode)
//{
//	m_meshNodes.clear();
//	return FindMeshNodesRecursive(pRootNode);
//}
//
//std::vector<rldx::DxMeshNode*> rldx::SceneGraphParser::GetResult()
//{
//	return m_meshNodes;
//}
//
//bool rldx::SceneGraphParser::FindMeshNodesRecursive(IDxSceneNode* pRootNode)
//{
//	bool bError = false;
//
//	if (pRootNode->GetType() == SceneNodeTypeEnum::Mesh)
//	{
//		DxMeshNode* pMeshNode = static_cast<DxMeshNode*>(pRootNode);
//		m_meshNodes.push_back(pMeshNode);
//	}
//
//	for (auto& child : pRootNode->GetChildren())
//	{
//		FindMeshNodesRecursive(child.get());
//	}
//
//	return true;
//}
//
//

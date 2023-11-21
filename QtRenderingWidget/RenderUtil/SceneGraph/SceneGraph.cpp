#include "SceneGraph.h"

using namespace Rldx;

DxSceneNode* SceneGraph::GetRootNode()
{
	 return m_spoRootNode.get(); 
}

void SceneGraph::DeleteNode(DxSceneNode* node)
{
	if(node->GetParent())
	{
		node->GetParent()->RemoveChild(node);
	}

	for(auto child : node->GetChildren())
	{
		DeleteNode(child.get());
	}
	
}

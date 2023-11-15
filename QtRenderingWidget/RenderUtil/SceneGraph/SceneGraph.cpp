#include "SceneGraph.h"

using namespace Rldx;

SceneNode* SceneGraph::GetRootNode()
{
	 return m_spoRootNode.get(); 
}

void SceneGraph::DeleteNode(SceneNode* node)
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

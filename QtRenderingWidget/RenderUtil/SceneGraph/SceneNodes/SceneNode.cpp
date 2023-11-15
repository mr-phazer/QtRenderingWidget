#include "SceneNode.h"

using namespace Rldx;

uint32_t SceneNode::sm_nextId = 100;



SceneNode::~SceneNode()
{
	m_wpoParent.lock()->RemoveChild(shared_from_this());	
}

inline ResourceTypeEnum Rldx::SceneNode::GetResourceType() 
{
	return m_resourceType;
}

SceneNode::SPtr SceneNode::Create(std::string name)
{	
	SPtr newNode= std::make_shared<SceneNode>();
	newNode->m_nodeId = GetNextId();	

	newNode->m_name = name == ("" ? "SceneNode" : name) + "__" + std::to_string(newNode->m_nodeId);	

	return newNode;
}


void SceneNode::AddChild(SceneNode::SPtr spoChild)
{		
	spoChild->SetParent(shared_from_this());	
	m_children.push_back(spoChild);
}

void SceneNode::AddChild(SceneNode* poChild)
{
	poChild->SetParent(shared_from_this());	
	m_children.push_back(SPtr(poChild));
}

void SceneNode::SetParent(SceneNode::SPtr parent)
{
	m_wpoParent = parent;
}

SceneNode* SceneNode::GetParent()
{	
	return m_wpoParent.lock().get();
}

void SceneNode::RemoveChild(SceneNode::SPtr child)
{
	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] == child)
		{
			RemoveChild(i);
			break;
		}
	}
}

void SceneNode::RemoveChild(SceneNode* poChild)
{
	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children[i].get() == poChild)
		{
			RemoveChild(i);
			break;
		}
	}
}

void SceneNode::RemoveChild(size_t index)
{
	m_children.erase(m_children.begin() + index);
}

void SceneNode::RemoveChildren()
{
	m_children.clear();
}

NodeTransform& SceneNode::GetTransform()
{
	return m_nodeTransform;
}

const NodeTransform& SceneNode::GetTransform() const
{
	return m_nodeTransform;
}

NodeId SceneNode::GetNextId()
{
	 return sm_nextId++; 
}

NodeId SceneNode::GetId()
{
	return m_nodeId;
}

SceneNode* SceneNode::GetChild(SceneNode* poChild)
{
	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children[i].get() == poChild)
		{
			return m_children[i].get();
		}
	}

	return nullptr;
}

SceneNode* SceneNode::GetChild(size_t index)
{
	if (index >= m_children.size())
	{
		return nullptr;
	}

	return m_children[index].get();
}

const std::vector<SceneNode::SPtr>& SceneNode::GetChildren() const
{
	return m_children;
}

std::vector<SceneNode::SPtr> SceneNode::GetChildren()
{
	return m_children;
}



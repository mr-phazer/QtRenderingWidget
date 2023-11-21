#include "DxSceneNode.h"

using namespace Rldx;

uint32_t DxSceneNode::sm_nextId = 100;



DxSceneNode::~DxSceneNode()
{
	m_wpoParent.lock()->RemoveChild(shared_from_this());	
}

inline ResourceTypeEnum Rldx::DxSceneNode::GetResourceType() 
{
	return m_resourceType;
}

DxSceneNode::SPtr DxSceneNode::Create(std::string name)
{	
	SPtr newNode= std::make_shared<DxSceneNode>();
	newNode->m_nodeId = GetNextId();	

	newNode->m_name = name == ("" ? "SceneNode" : name) + "__" + std::to_string(newNode->m_nodeId);	

	return newNode;
}


void DxSceneNode::AddChild(DxSceneNode::SPtr spoChild)
{		
	spoChild->SetParent(shared_from_this());	
	m_children.push_back(spoChild);
}

void DxSceneNode::AddChild(DxSceneNode* poChild)
{
	poChild->SetParent(shared_from_this());	
	m_children.push_back(SPtr(poChild));
}

void DxSceneNode::SetParent(DxSceneNode::SPtr parent)
{
	m_wpoParent = parent;
}

DxSceneNode* DxSceneNode::GetParent()
{	
	return m_wpoParent.lock().get();
}

void DxSceneNode::RemoveChild(DxSceneNode::SPtr child)
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

void DxSceneNode::RemoveChild(DxSceneNode* poChild)
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

void DxSceneNode::RemoveChild(size_t index)
{
	m_children.erase(m_children.begin() + index);
}

void DxSceneNode::RemoveChildren()
{
	m_children.clear();
}

NodeTransform& DxSceneNode::GetTransform()
{
	return m_nodeTransform;
}

const NodeTransform& DxSceneNode::GetTransform() const
{
	return m_nodeTransform;
}

NodeId DxSceneNode::GetNextId()
{
	 return sm_nextId++; 
}

NodeId DxSceneNode::GetId()
{
	return m_nodeId;
}

DxSceneNode* DxSceneNode::GetChild(DxSceneNode* poChild)
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

DxSceneNode* DxSceneNode::GetChild(size_t index)
{
	if (index >= m_children.size())
	{
		return nullptr;
	}

	return m_children[index].get();
}

// TODO: test this
inline DxSceneNode* Rldx::DxSceneNode::FindNode(DxSceneNode* nodeToFind, DxSceneNode* currentNode)
{
	if (currentNode == nodeToFind) {
		return currentNode;
	}

	for (auto child : currentNode->GetChildren())
	{
		auto result = FindNode(nodeToFind, child.get());

		if (result != nullptr)
			return result;
	}

	return nullptr;
}

const std::vector<DxSceneNode::SPtr>& DxSceneNode::GetChildren() const
{
	return m_children;
}

std::vector<DxSceneNode::SPtr>& DxSceneNode::GetChildren()
{
	return m_children;
}



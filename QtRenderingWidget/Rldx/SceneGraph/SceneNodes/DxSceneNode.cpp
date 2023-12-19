#include "DxSceneNode.h"

using namespace Rldx;

uint32_t IIdentifiable::sm_nextId = 1;

DxSceneNode::~DxSceneNode()
{
	m_wpoParent->RemoveChild(this);	
}

Rldx::ResourceTypeEnum Rldx::DxSceneNode::GetResourceType() 
{
	return m_resourceType;
}

Rldx::DxSceneNode::Sptr DxSceneNode::Create(std::string name)
{	
	auto newNode = std::make_shared<DxSceneNode>();
	newNode->SetName(name);

	return newNode;
}

void Rldx::DxSceneNode::SetName(const std::string& name)
{
	m_name = GetTypeString() + "__" + name + "_" + std::to_string(GetId());
}


void DxSceneNode::AddChild(DxSceneNode::Sptr spoChild)
{		
	spoChild->SetParent(this);
	m_children.push_back(spoChild);
}

void DxSceneNode::SetParent(DxSceneNode* poParent)
{
	m_wpoParent = poParent;
}

DxSceneNode* DxSceneNode::GetParent()
{	
	return m_wpoParent;
}

void DxSceneNode::RemoveChild(const DxSceneNode::Sptr& child)
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

DxSceneNode* DxSceneNode::GetChildByPtr(DxSceneNode* poChild)
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

size_t Rldx::DxSceneNode::GetChildCount() const
{
	return m_children.size();
}

// TODO: test this
DxSceneNode* Rldx::DxSceneNode::FindChild(DxSceneNode* nodeToFind, DxSceneNode* currentNode)
{
	// to break out of recursion
	if (currentNode == nodeToFind) {
		return currentNode;
	}

	for (auto& childNode : currentNode->GetChildren())
	{
		auto result = FindChild(nodeToFind, childNode.get());

		if (result != nullptr)
			return result;
	}

	return nullptr;
}




const std::vector<DxSceneNode::Sptr>& DxSceneNode::GetChildren() const
{
	return m_children;
}

std::vector<DxSceneNode::Sptr>& DxSceneNode::GetChildren()
{
	return m_children;
}

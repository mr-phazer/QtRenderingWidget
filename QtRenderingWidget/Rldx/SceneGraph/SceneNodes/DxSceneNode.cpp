#include "DxSceneNode.h"

using namespace Rldx;



IDxSceneNode::~IDxSceneNode()
{
	m_wpoParent->RemoveChild(this);	
}


Rldx::IDxSceneNode::Sptr DxSceneNode::Create(std::string name)
{	
	auto newNode = std::make_shared<DxSceneNode>();
	newNode->SetName(name);

	return newNode;
}



void IDxSceneNode::AddChild(IDxSceneNode::Sptr spoChild)
{		
	spoChild->SetParent(this);
	m_children.push_back(spoChild);
}

void IDxSceneNode::SetParent(IDxSceneNode* poParent)
{
	m_wpoParent = poParent;
}

//std::string Rldx::IDxSceneNode::GetTypeString() const
//{
//	return "DxScene";
//}

IDxSceneNode* IDxSceneNode::GetParent()
{	
	return m_wpoParent;
}

void IDxSceneNode::RemoveChild(const IDxSceneNode::Sptr& child)
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

void IDxSceneNode::RemoveChild(IDxSceneNode* poChild)
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

void IDxSceneNode::RemoveChild(size_t index)
{
	m_children.erase(m_children.begin() + index);
}

void IDxSceneNode::RemoveChildren()
{
	m_children.clear();
}

NodeTransform& IDxSceneNode::GetTransform()
{
	return m_nodeTransform;
}

const NodeTransform& IDxSceneNode::GetTransform() const
{
	return m_nodeTransform;
}

IDxSceneNode* IDxSceneNode::GetChildByPtr(IDxSceneNode* poChild)
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

IDxSceneNode* IDxSceneNode::GetChild(size_t index)
{
	if (index >= m_children.size())
	{
		return nullptr;
	}

	return m_children[index].get();
}

size_t Rldx::IDxSceneNode::GetChildCount() const
{
	return m_children.size();
}

// TODO: test this
IDxSceneNode* Rldx::IDxSceneNode::FindChild(IDxSceneNode* nodeToFind, IDxSceneNode* currentNode)
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




const std::vector<IDxSceneNode::Sptr>& IDxSceneNode::GetChildren() const
{
	return m_children;
}

std::vector<IDxSceneNode::Sptr>& IDxSceneNode::GetChildren()
{
	return m_children;
}

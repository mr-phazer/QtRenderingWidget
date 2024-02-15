//#include "DxSceneNode.h"
//#include <d3d11.h>
//
//
//using namespace rldx;
//
//
//
////IDxSceneNode::~IDxSceneNode()
////{
////	if (m_poParent) {
////		m_poParent->RemoveChild(this);
////	};	
////}
//
//void rldx::DxSceneNode::SetName(const std::string& name)
//{
//	m_name = this->GetTypeString() + " # " + name + " # " + std::to_string(GetId());
//}
//
//
//rldx::DxSceneNode::Tre rldx::DxSceneNode::Create(std::string name)
//{
//	auto newNode = std::make_shared<DxSceneNode>();
//	newNode->SetName(name);
//
//	return newNode;
//}
//
//void rldx::DxSceneNode::Update(float timeElapsed)
//{
//}
//
//void rldx::DxSceneNode::FlushToRenderBucket(IRenderBucket* pRenderQueue)
//{
//}
//
//
////std::string rldx::IDxSceneNode::GetTypeString() const
////{
////	return "DxScene";
////}
//
//IDxSceneNode* IDxSceneNode::GetParent()
//{	
//	return m_poParent;
//}
//
//void IDxSceneNode::RemoveChild(const IDxSceneNode::SharedPointerBase& child)
//{
//	for (size_t i = 0; i < m_children.size(); i++)
//	{
//		if (m_children[i] == child)
//		{
//			RemoveChild(i);
//			break;
//		}
//	}
//}
//
//void IDxSceneNode::RemoveChild(IDxSceneNode* poChild)
//{
//	for (size_t i = 0; i < m_children.size(); i++)
//	{
//		if (m_children[i].get() == poChild)
//		{
//			RemoveChild(i);
//			break;
//		}
//	}
//}
//
//void IDxSceneNode::RemoveChild(size_t index)
//{
//	m_children.erase(m_children.begin() + index);
//}
//
//void IDxSceneNode::RemoveChildren()
//{
//	m_children.clear();
//}
//
////NodeTransform& IDxSceneNode::TreeNode::GetLocalTransform()
////{
////	return m_nodeTransform;
////}
//// TodoalTransform()
//{
//	// TODO: insert return statement here
//}
//
//const NodeTransform& DxSceneNode::GetLocalTransform() const
//{
//	return m_nodeTransform;
//}
//
////IDxSceneNode* IDxSceneNode::GetChildByPtr(IDxSceneNode* poChild)
////{
////	for (size_t i = 0; i < m_children.size(); i++)
////	{
////		if (m_children[i].get() == poChild)
////		{
////			return m_children[i].get();
////		}
////	}
////
////	return nullptr;
////}
////
////IDxSceneNode* IDxSceneNode::GetChild(size_t index)
////{
////	if (index >= m_children.size())
////	{
////		return nullptr;
////	}
////
////	return m_children[index].get();
////}
////
////size_t rldx::IDxSceneNode::GetChildCount() const
////{
////	return m_children.size();
////}
////
////// TODO: test this
////IDxSceneNode* rldx::IDxSceneNode::FindChild(IDxSceneNode* nodeToFind, IDxSceneNode* currentNode)
////{
////	// to break out of recursion
////	if (currentNode == nodeToFind) {
////		return currentNode;
////	}
////
////	for (auto& childNode : currentNode->GetChildren())
////	{
////		auto result = FindChild(nodeToFind, childNode.get());
////
////		if (result != nullptr)
////			return result;
////	}
////
////	return nullptr;
////}
////
////
////
////
////const std::vector<IDxSceneNode::SharedPointerBase>& IDxSceneNode::GetChildren() const
////{
////	return m_children;
////}
////
////std::vector<IDxSceneNode::SharedPointerBase>& IDxSceneNode::GetChildren()
////{
////	return m_children;
////}

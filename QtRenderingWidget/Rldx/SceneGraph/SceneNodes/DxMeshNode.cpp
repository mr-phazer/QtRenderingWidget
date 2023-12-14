#include "DxMeshNode.h"

Rldx::DxMeshNode::Sptr Rldx::DxMeshNode::Create(const std::string& name)
{
	auto newMeshNode = std::shared_ptr<DxMeshNode>(new DxMeshNode(name));

	return newMeshNode;
}
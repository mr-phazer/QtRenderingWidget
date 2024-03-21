#include "DxBaseNode.h"

std::wstring rldx::DxBaseNode::GetTypeString() const
{
	return L"DxBaseNode";
}

rldx::SceneNodeTypeEnum rldx::DxBaseNode::GetType() const
{
	return rldx::SceneNodeTypeEnum::EmptyNode;
}

void rldx::DxBaseNode::SetDeformerNodeRecursive(const rldx::DxDeformerNode* poDeformerNode, int32_t boneIndex)
{
	SetDeformerNode(poDeformerNode, boneIndex);

	for (auto& itChild : GetChildren())
	{
		itChild->SetDeformerNodeRecursive(poDeformerNode, boneIndex);
	}
}

void rldx::DxBaseNode::Update(float timeElapsed)
{
	UppdateGlobalTransform(timeElapsed);
}

void rldx::DxBaseNode::FlushToRenderBucket(IRenderBucket* pRenderQueue)
{
	// TODO: add small cube to renderqueue, cube for specifying node translation
}

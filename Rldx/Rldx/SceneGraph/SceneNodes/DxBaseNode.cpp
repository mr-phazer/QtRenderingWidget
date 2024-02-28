#include "DxBaseNode.h"

std::wstring rldx::DxBaseNode::GetTypeString() const
{
	return L"DxBaseNode";
}

rldx::SceneNodeTypeEnum rldx::DxBaseNode::GetType() const
{
	return rldx::SceneNodeTypeEnum::EmptyNode;
}

void rldx::DxBaseNode::Update(float timeElapsed)
{
	UppdateGlobalTransform(timeElapsed);
}

void rldx::DxBaseNode::FlushToRenderBucket(IRenderBucket* pRenderQueue)
{
	// TODO: add small cube to renderqueue, cube for specifying node position
}

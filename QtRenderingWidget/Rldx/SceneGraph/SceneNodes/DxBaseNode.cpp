#include "DxBaseNode.h"

std::string rldx::DxBaseNode::GetTypeString() const
{
    return std::string();
}

rldx::SceneNodeTypeEnum rldx::DxBaseNode::GetType() const
{
    return rldx::SceneNodeTypeEnum::EmptyNode;
}

void rldx::DxBaseNode::Update(float timeElapsed)
{
    UppdateGlobalTransform(timeElapsed);
}

void rldx::DxBaseNode::FlushToRenderQueue(IRenderBucket* pRenderQueue)
{
    // TODO: add small cube to reneder cue, cube for specifying node position
}

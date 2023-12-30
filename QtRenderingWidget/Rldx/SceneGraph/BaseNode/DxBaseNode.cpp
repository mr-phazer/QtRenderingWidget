#include "DxBaseNode.h"

std::string Rldx::DxBaseNode::GetTypeString() const
{
    return std::string();
}

Rldx::SceneNodeTypeEnum Rldx::DxBaseNode::GetType() const
{
    return Rldx::SceneNodeTypeEnum::EmptyNode;
}

void Rldx::DxBaseNode::Update(float timeElapsed)
{
}

void Rldx::DxBaseNode::FlushToRenderQueue(IDxRenderQueue* pRenderQueue)
{
}

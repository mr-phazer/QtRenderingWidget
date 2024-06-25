#include "SceneGraph.h"

namespace rldx
{
	DxBaseNode* DxSceneGraph::GetRootNode() const
	{
		return m_vmdRootNode.get();
	}

	/// <summary>
	/// Get a bounding box that is precisely big enough to contain all the meshes in the in the scene graph
	/// </summary>
	/// <returns></returns>

	DirectX::BoundingBox DxSceneGraph::GetRootBoundBox()
	{
		UpdateBoundBoxRecursive(GetRootNode());

		return m_SceneBoundBox;
	}

	void DxSceneGraph::UpdateNodes(float timeElapsed)
	{
		UpdateNodesRecursive(m_vmdRootNode.get(), timeElapsed);
	}

	void DxSceneGraph::UpdateNodes(DxBaseNode* pNode, float timeElapsed)
	{
		UpdateNodesRecursive(pNode, timeElapsed);
	}

	void DxSceneGraph::FillRenderBucket(IRenderBucket* pDestRenderBucket)
	{
		FillRenderBucketRecursive(m_vmdRootNode.get(), pDestRenderBucket);
	}

	void DxSceneGraph::AddNodeToLinearIndexTable(DxBaseNode* node)
	{
		nodeLookUptable.resize(
			static_cast<size_t>(node->GetId()) >= nodeLookUptable.size() ?
			static_cast<size_t>(node->GetId()) + 1 : nodeLookUptable.size()
		);

		nodeLookUptable[node->GetId()] = node;
	}

	void DxSceneGraph::FillRenderBucketRecursive(DxBaseNode* pNode, IRenderBucket* pDestRenderQueue)
	{
		pNode->FlushToRenderBucket(pDestRenderQueue);

		for (auto& itChildNode : pNode->GetChildren())
		{
			FillRenderBucketRecursive(itChildNode.get(), pDestRenderQueue);
		};
	}

	void DxSceneGraph::UpdateNodesRecursive(DxBaseNode* pRootNode, float timeElapsed)
	{
		pRootNode->Update(timeElapsed);

		for (auto& itChildNode : pRootNode->GetChildren())
		{
			UpdateNodesRecursive(itChildNode.get(), timeElapsed);
		};
	}

	void DxSceneGraph::UpdateBoundBoxRecursive(DxBaseNode* node)
	{
		for (auto& itChildeNode : node->GetChildren())
		{
			UpdateBoundBoxRecursive(itChildeNode.get());
		}

		// Scene Graphs's BoundBox has to contain all node bound boxes:
		DirectX::BoundingBox::CreateMerged(
			m_SceneBoundBox,
			m_SceneBoundBox,
			node->GetNodeBoundingBox());
	}
}
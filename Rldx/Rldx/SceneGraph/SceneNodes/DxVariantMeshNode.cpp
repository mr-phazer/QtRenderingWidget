#include "..\..\Interfaces\IRenderBucket.h"
#include "DxVariantMeshNode.h"


namespace rldx
{
	void DxVariantMeshNode::FlushToRenderBucket(IRenderBucket* pRenderQueue)
	{
		UpdateGlobalTransform(0.0f);
		auto mNodwWorld = GetCurrentGlobalTransForm().Transpose();

		for (auto& itModelData : m_models)
		{
			itModelData->SetNodeWorldTransForm(mNodwWorld);
			itModelData->FlushModelMeshesToRenderBucked(pRenderQueue);
		}

		if (!m_poDeformerNode) return;

		// add skeleton "stick figure" mesh from deformer node
		if (m_poDeformerNode->GetDrawState() == DrawStateEnum::Draw) {
			pRenderQueue->AddItem(&m_poDeformerNode->MeshRenderData());
		}
	};
}; // namespace rldx



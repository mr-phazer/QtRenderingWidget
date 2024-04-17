#include "..\..\Interfaces\IRenderBucket.h"
#include "DxVariantMeshNode.h"


namespace rldx
{
	void DxVariantMeshNode::FlushToRenderBucket(IRenderBucket* pRenderQueue)
	{
		UpdateGlobalTransform(0.0f);
		auto mNodwWorld = GetCurrentGlobalTransForm().Transpose();

		for (auto& itMeshData : m_meshQueue)
		{
			itMeshData->perMesh_VS_CB.data.mNodeWorld = mNodwWorld;
			pRenderQueue->AddItem(itMeshData.get());
		}
	};
}; // namespace rldx



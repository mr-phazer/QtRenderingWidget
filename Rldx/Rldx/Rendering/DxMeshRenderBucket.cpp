#include "DxMeshRenderBucket.h"

#include "..\Interfaces\IRenderQueueItem.h"
#include "DxShaderProgram.h"

void rldx::DxMeshRenderBucket::AddItem(IRenderQueueItem* renderItem)
{
	m_renderItems.push_back(renderItem);
}

void rldx::DxMeshRenderBucket::ClearItems() {
	m_renderItems.clear();
}

void rldx::DxMeshRenderBucket::Draw(ID3D11DeviceContext* poDC, DxMeshShaderProgram* defaultShaderProgram)
{
	for (auto& itItem : m_renderItems)
	{
		// ready shader program
		itItem->BindToDC(poDC);

		// draw mesh
		itItem->Draw(poDC);

		itItem->UnbindFromDC(poDC);
	};

	// -- clear queue after each full draw
	ClearItems();
}

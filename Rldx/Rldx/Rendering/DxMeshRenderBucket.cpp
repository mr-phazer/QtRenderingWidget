#include "DxMeshRenderBucket.h"

#include "..\Rendering\DxMeshRenderData.h"
#include "DxShaderProgram.h"

void rldx::DxMeshRenderBucket::AddItem(DxMeshRenderingData* renderItem)
{
	m_renderItems.push_back(renderItem);
}

void rldx::DxMeshRenderBucket::ClearItems()
{
	m_renderItems.clear();
}

void rldx::DxMeshRenderBucket::CopyItemsTo(IRenderBucket* pOtherBucket)
{
	for (auto& itItem : m_renderItems)
	{
		pOtherBucket->AddItem(itItem);
	};

	ClearItems();
}

void rldx::DxMeshRenderBucket::Draw(ID3D11DeviceContext* poDC, DxMeshShaderProgram* defaultShaderProgram)
{
	// TODO: DEBUG find out why this keeps running after assets are deleted?
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

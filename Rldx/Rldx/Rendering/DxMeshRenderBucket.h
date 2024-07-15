#pragma once

#include <vector>
#include "..\Interfaces\IRenderBucket.h"

struct ID3D11DeviceContext;

namespace rldx {

	struct DxMeshRenderingData;
	class DxMeshShaderProgram;

	class DxMeshRenderBucket : public IRenderBucket
	{
		std::vector<DxMeshRenderingData*> m_renderItems;

	public:
		virtual void AddItem(DxMeshRenderingData* renderItem) override;
		virtual void ClearItems() override;
		const std::vector<DxMeshRenderingData*>& GetItems() { return m_renderItems; }
		void CopyItemsTo(IRenderBucket* pOtherBucket);
		void Draw(ID3D11DeviceContext* poDC, DxMeshShaderProgram* defaultShaderProgram = nullptr);
	};
}
#pragma once

#include <vector>
#include "..\Interfaces\IRenderBucket.h"

struct ID3D11DeviceContext;

namespace rldx {

	class IRenderQueueItem;
	class DxMeshShaderProgram;

	class DxMeshRenderBucket : public IRenderBucket
	{
		std::vector<IRenderQueueItem*> m_renderItems;

	public:
		virtual void AddItem(IRenderQueueItem* renderItem) override;;
		virtual void ClearItems() override;;
		void Draw(ID3D11DeviceContext* poDC, DxMeshShaderProgram* defaultShaderProgram = nullptr);
	};
}
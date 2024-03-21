#pragma once

#include <vector>
#include "..\Interfaces\IRenderBucket.h"

struct ID3D11DeviceContext;

namespace rldx {

	struct DxMeshData;
	class DxMeshShaderProgram;

	class DxMeshRenderBucket : public IRenderBucket
	{
		std::vector<DxMeshData*> m_renderItems;

	public:
		virtual void AddItem(DxMeshData* renderItem) override;;
		virtual void ClearItems() override;;
		void Draw(ID3D11DeviceContext* poDC, DxMeshShaderProgram* defaultShaderProgram = nullptr);
	};
}
#pragma once

#include <cassert>
#include <d3d11.h>
#include <typeinfo>
#include <wrl/client.h>

#include "..\..\..\DirectXTK\Inc\GeometricPrimitive.h"
#include "..\..\..\DirectXTK\Inc\VertexTypes.h"
#include "..\Managers\ResourceManager\DxResourceManager.h"

#include "..\DataTypes\DxMeshData.h"
#include "..\Interfaces\IDrawable.h"
#include "DxMaterial.h"
#include "DxShaderProgram.h"

namespace rldx {

	class DxMesh : public IDxResource, public IDrawable, public IBindable
	{
	public:
		void Draw(ID3D11DeviceContext* poDC) override
		{
			poDC->DrawIndexed(m_poMesh.indexCount, 0, 0);
		};

		ResourceTypeEnum GetType() const override;
		std::wstring GetTypeString() const override;

		void SetMeshData(const DxCommonMeshData& meshData);


		void BindToDC(ID3D11DeviceContext* poDC)
		{
			UINT stride = sizeof(CommonVertex);
			UINT offset = 0;
			poDC->IASetVertexBuffers(0, 1, m_poMesh.cpoVertexBuffer.GetAddressOf(), &stride, &offset);
			poDC->IASetIndexBuffer(m_poMesh.cpoIndexBuffer.Get(), m_poMesh.indexFormat, 0);
			poDC->IASetPrimitiveTopology(m_poMesh.primitiveTopology);
		};

	private:
		DxCommonMeshData m_poMesh;
	};

}; // namespace rldx

#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <typeinfo>
#include <cassert>

#include "..\..\..\DirectXTK\Inc\GeometricPrimitive.h"
#include "..\..\..\DirectXTK\Inc\VertexTypes.h"
#include "..\Managers\ResourceManager\DxResourceManager.h"

#include "..\Interfaces\IDrawable.h"
#include "DxShaderProgram.h"
#include "DxMaterial.h"
#include "..\DataTypes\DxMeshData.h"

namespace Rldx {

	class DxMesh : public IDxResource, public IDrawable, public IBindable
	{
	public:
		void Draw(ID3D11DeviceContext* poDC) override
		{	
			BindToDC(poDC);

			poDC->DrawIndexed(m_meshData.indexCount, 0, 0);
		};
		
		ResourceTypeEnum GetType() const override;		
		std::string GetTypeString() const override;
		
		void SetMeshData(const DxCommonMeshData& meshData);				
	
	private:
		void BindToDC(ID3D11DeviceContext* poDC)
		{
			UINT stride = sizeof(CommonVertex);
			UINT offset = 0;
			poDC->IASetVertexBuffers(0, 1, m_meshData.cpoVertexBuffer.GetAddressOf(), &stride, &offset);
			poDC->IASetIndexBuffer(m_meshData.cpoIndexBuffer.Get(), m_meshData.indexFormat, 0);
			poDC->IASetPrimitiveTopology(m_meshData.primitiveTopology);
		};

	private:
		DxCommonMeshData m_meshData;		
	};

}; // namespace Rldx

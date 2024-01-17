#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <typeinfo>
#include <cassert>

#include "..\DirectXTK\Inc\GeometricPrimitive.h"
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
			// TODO: rmove?
			//BindToDC(poDC);

			poDC->DrawIndexed(m_poMesh.indexCount, 0, 0);
		};
		
		ResourceTypeEnum GetType() const override;		
		std::string GetTypeString() const override;
		
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

}; // namespace Rldx

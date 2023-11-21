#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <typeinfo>
#include <cassert>

#include "..\..\..\DirectXTK\Inc\GeometricPrimitive.h"
#include "..\..\..\DirectXTK\Inc\VertexTypes.h"

using namespace DirectX;


namespace Rldx {

	struct CommonVertex;


	class DxMeshData
	{
	public:
		void Draw(ID3D11DeviceContext* poDeviceContext,  ID3D11RenderTargetView* destRtV  DxShaderProgram)
		{
			poDeviceContext->OMSetRenderTargets(1, &destRtV, nullptr);			

			
			// TODO: finish
		}



	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_cpoIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_cpoVertexBuffer;
		
	};
    
	template <typename INDEX_TYPE, typename VERTEX_TYPE, >
	class DxMeshCreator
	{	
		static CreateMesh();


		template <typename INDEX_TYPE>
		bool FillIndexBuffer(ID3D11Device* _poDevice, uint32_t indexCount, const C* pIndices);

		template <typename VERTEX_TYPE>
		bool FillVertexBuffer(ID3D11Device* _poDevice, uint32_t vertexCount, const VERTEX_TYPE* pVertices);
		
	


	}; // class DxMeshCreator

	template<typename INDEX_TYPE>
	inline DxMeshCreator:: DxMeshCreator::FillIndexBuffer(ID3D11Device* _poDevice, uint32_t indexCount, const INDEX_TYPE* pIndices)
	{
		if (indexCount == 0 || pIndices == nullptr)
			return false;

		// Setup the buffer description for "index buffer"
		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT; //D3D11_USAGE::D3D11_USAGE_DYNAMIC; // TODO: change to "static" = ?
		vertexBufferDesc.ByteWidth = indexCount * sizeof(INDEX_TYPE);
		vertexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0; // D3D11_CPU_ACCESS_WRITE; // TODO, change to "no CPU access"?
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Setup init data
		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = pIndices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		HRESULT hr = _poDevice->CreateBuffer(&vertexBufferDesc, &indexData, &m_cpoIndexBuffer);
		assert(SUCCEEDED(hr));

		return m_cpoIndexBuffer;
	}

	template<typename VERTEX_TYPE>
	inline bool DxMeshCreator::FillVertexBuffer(ID3D11Device* _poDevice, uint32_t vertexCount, const VERTEX_TYPE* pVertices)
	{
		if (_data_size == 0)
			return false;

		// Setup the buffer descript for "vertex buffer"
		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = vertexCount * sizeof(VERTEX_TYPE);
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Setup init data
		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = _pvData;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		HRESULT hr = _poDevice->CreateBuffer(&vertexBufferDesc, (vertexData.pSysMem) ? &vertexData : NULL, &m_cpoVertexBuffer);
		assert(SUCCEEDED(hr));

		return SUCCEEDED(hr);
	}

}; // namespace Rldx


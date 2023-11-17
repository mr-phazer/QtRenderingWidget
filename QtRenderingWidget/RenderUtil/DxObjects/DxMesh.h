#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <typeinfo>
#include <cassert>

namespace Rldx {

	struct CommonVertex;

	class DxMesh
	{
	public:
		void Draw(ID3D11DeviceContext poDevice);

		template <typename INDEX_TYPE>
		bool FillIndexBuffer(ID3D11Device* _poDevice, uint32_t indexCount, const INDEX_TYPE* pIndices);

		template <typename VERTEX_TYPE>
		bool FillVertexBuffer(ID3D11Device* _poDevice, uint32_t vertexCount, const VERTEX_TYPE* pVertices)
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

	private:
		// ---  vertices + indices ComPtr smart pointers for COM objects
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_cpoVertexBuffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_cpoIndexBuffer = nullptr;
	};

	template<typename INDEX_TYPE>
	inline bool DxMesh::FillIndexBuffer(ID3D11Device* _poDevice, uint32_t indexCount, const INDEX_TYPE* pIndices)
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

		return SUCCEEDED(hr);
	}

}; // namespace Rldx


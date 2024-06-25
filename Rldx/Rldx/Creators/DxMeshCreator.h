#pragma once

#include <assert.h>
#include "..\DataTypes\DxMeshData.h"

namespace rldx {

	template<typename VERTEX_TYPE, typename INDEX_TYPE>
	class DxMeshRenderDataCreator
	{
	public:
		TDxMeshRenderData<VERTEX_TYPE, INDEX_TYPE> CreateDxMeshRenderData(ID3D11Device* _poDevice, const std::vector<VERTEX_TYPE>& vertices, const std::vector<INDEX_TYPE>& indices);

	private:
		bool FillIndexBuffer(ID3D11Device* _poDevice, uint32_t indexCount, const INDEX_TYPE* pIndices);
		bool FillVertexBuffer(ID3D11Device* _poDevice, uint32_t vertexCount, const VERTEX_TYPE* pVertices);
	private:
		TDxMeshRenderData<VERTEX_TYPE, INDEX_TYPE> m_poMeshBuffers;

	}; // class DxMeshCreatorHelper

	template<typename VERTEX_TYPE, typename INDEX_TYPE>

	inline TDxMeshRenderData<VERTEX_TYPE, INDEX_TYPE> DxMeshRenderDataCreator<VERTEX_TYPE, INDEX_TYPE>::CreateDxMeshRenderData(
		ID3D11Device* _poDevice,
		const std::vector<VERTEX_TYPE>& vertices,
		const std::vector<INDEX_TYPE>& indices)
	{
		m_poMeshBuffers.originalMeshData.vertices = vertices;
		m_poMeshBuffers.originalMeshData.indices = indices;

		FillVertexBuffer(_poDevice, static_cast<UINT>(vertices.size()), vertices.data());
		FillIndexBuffer(_poDevice, static_cast<UINT>(indices.size()), indices.data());

		m_poMeshBuffers.indexCount = static_cast<uint32_t>(indices.size());
		m_poMeshBuffers.indexFormat = sizeof(INDEX_TYPE) == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
		m_poMeshBuffers.primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		return m_poMeshBuffers;
	}

	template<typename VERTEX_TYPE, typename INDEX_TYPE>
	inline bool DxMeshRenderDataCreator<VERTEX_TYPE, INDEX_TYPE>::FillIndexBuffer(ID3D11Device* _poDevice, uint32_t indexCount, const INDEX_TYPE* pIndices)
	{
		if (indexCount == 0 || pIndices == nullptr)
			return false;

		// Setup the buffer description for "index buffer"
		D3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT; //D3D11_USAGE::D3D11_USAGE_DYNAMIC; // TODO: change to "static" = ?
		vertexBufferDesc.ByteWidth = indexCount * sizeof(INDEX_TYPE);
		vertexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0; // D3D11_CPU_ACCESS_WRITE; // TODO, change to "no CPU access"?
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Setup init data
		D3D11_SUBRESOURCE_DATA indexData{};
		indexData.pSysMem = pIndices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		HRESULT hr = _poDevice->CreateBuffer(&vertexBufferDesc, &indexData, &m_poMeshBuffers.cpoIndexBuffer);
		assert(SUCCEEDED(hr));

		return SUCCEEDED(hr);
	}

	template<typename VERTEX_TYPE, typename INDEX_TYPE>
	inline bool DxMeshRenderDataCreator<VERTEX_TYPE, INDEX_TYPE>::FillVertexBuffer(ID3D11Device* _poDevice, uint32_t vertexCount, const VERTEX_TYPE* pVertices)
	{
		if (vertexCount == 0)
			return false;

		// Setup the buffer descript for "vertex buffer"
		D3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = vertexCount * sizeof(VERTEX_TYPE);
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Setup init data
		D3D11_SUBRESOURCE_DATA vertexData{};
		vertexData.pSysMem = pVertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		HRESULT hr = _poDevice->CreateBuffer(&vertexBufferDesc, (vertexData.pSysMem) ? &vertexData : NULL, &m_poMeshBuffers.cpoVertexBuffer);
		assert(SUCCEEDED(hr));

		return SUCCEEDED(hr);
	}
}; // namespace rldx

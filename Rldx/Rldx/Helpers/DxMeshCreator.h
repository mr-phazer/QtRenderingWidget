#pragma once

#include <assert.h>
#include "..\DataTypes\DxMeshData.h"
#include "..\..\ImportExport\RigidModel\DataTypes\RigidModelFile.h"

namespace rldx {

	template<typename VERTEX_TYPE, typename INDEX_TYPE>
	class DxMeshDataCreator
	{
	public:
		TDxMeshData<VERTEX_TYPE, INDEX_TYPE> CreateDxMeshData(ID3D11Device* _poDevice, const std::vector<VERTEX_TYPE>& vertices, const std::vector<INDEX_TYPE>& indices);

	private:
		bool FillIndexBuffer(ID3D11Device* _poDevice, uint32_t indexCount, const INDEX_TYPE* pIndices);
		bool FillVertexBuffer(ID3D11Device* _poDevice, uint32_t vertexCount, const VERTEX_TYPE* pVertices);
	private:
		TDxMeshData<VERTEX_TYPE, INDEX_TYPE> m_poMesh;

	}; // class DxMeshCreatorHelper

	template<typename VERTEX_TYPE, typename INDEX_TYPE>

	inline TDxMeshData<VERTEX_TYPE, INDEX_TYPE> DxMeshDataCreator<VERTEX_TYPE, INDEX_TYPE>::CreateDxMeshData(
		ID3D11Device* _poDevice,
		const std::vector<VERTEX_TYPE>& vertices,
		const std::vector<INDEX_TYPE>& indices)
	{
		m_poMesh.originalMeshData.vertices = vertices;
		m_poMesh.originalMeshData.indices = indices;

		FillVertexBuffer(_poDevice, static_cast<UINT>(vertices.size()), vertices.data());
		FillIndexBuffer(_poDevice, static_cast<UINT>(indices.size()), indices.data());

		m_poMesh.indexCount = static_cast<uint32_t>(indices.size());
		m_poMesh.indexFormat = sizeof(INDEX_TYPE) == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
		m_poMesh.primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		return m_poMesh;
	}

	template<typename VERTEX_TYPE, typename INDEX_TYPE>
	inline bool DxMeshDataCreator<VERTEX_TYPE, INDEX_TYPE>::FillIndexBuffer(ID3D11Device* _poDevice, uint32_t indexCount, const INDEX_TYPE* pIndices)
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
		HRESULT hr = _poDevice->CreateBuffer(&vertexBufferDesc, &indexData, &m_poMesh.cpoIndexBuffer);
		assert(SUCCEEDED(hr));

		return SUCCEEDED(hr);
	}

	template<typename VERTEX_TYPE, typename INDEX_TYPE>
	inline bool DxMeshDataCreator<VERTEX_TYPE, INDEX_TYPE>::FillVertexBuffer(ID3D11Device* _poDevice, uint32_t vertexCount, const VERTEX_TYPE* pVertices)
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
		HRESULT hr = _poDevice->CreateBuffer(&vertexBufferDesc, (vertexData.pSysMem) ? &vertexData : NULL, &m_poMesh.cpoVertexBuffer);
		assert(SUCCEEDED(hr));

		return SUCCEEDED(hr);
	}
	// TODO: remove once tested

	class DxMeshCreatorHelper
	{
	public:
		static DxCommonMeshData Create(ID3D11Device* poDevice, const std::vector<CommonVertex>& vertices, const std::vector<uint16_t>& indices);
		static DxCommonMeshData MakeTestCubeMesh(ID3D11Device* poDevice);
		static DxCommonMeshData MakeGrid(ID3D11Device* poDevice, int linesPerAxis = 10, float spacing = 0.01f);

		static DxCommonMeshData CreateFromRmv2Mesh(ID3D11Device* poDevice, const rmv2::MeshBlockCommon& rmv2Mesh);
	};

	class DxModelCreatorHelper
	{
		static std::vector<DxCommonMeshData> CreateFromRmv2File(ID3D11Device* poDevice, const rmv2::RigidModelFileCommon& rmv2Model);
	};

}; // namespace rldx



using byte = std::uint8_t;
using word = std::uint16_t;
using dword = std::uint32_t;
#pragma once

#include "..\Types\DxMeshData.h"
#include "..\..\..\DirectXTK\Inc\GeometricPrimitive.h"

namespace Rldx {

	template<typename VERTEX_TYPE, typename INDEX_TYPE>
	class DxMeshDataCreator
	{
	public:
		TDxMeshData<VERTEX_TYPE, INDEX_TYPE> CreateDxMeshData(ID3D11Device* _poDevice, const std::vector<VERTEX_TYPE>& vertices, const std::vector<INDEX_TYPE>& indices);

	private:
		bool FillIndexBuffer(ID3D11Device* _poDevice, uint32_t indexCount, const INDEX_TYPE* pIndices);
		bool FillVertexBuffer(ID3D11Device* _poDevice, uint32_t vertexCount, const VERTEX_TYPE* pVertices);
	private:
		TDxMeshData<VERTEX_TYPE, INDEX_TYPE> m_meshData;

	}; // class DxMeshCreator

	template<typename VERTEX_TYPE, typename INDEX_TYPE>

	inline TDxMeshData<VERTEX_TYPE, INDEX_TYPE> DxMeshDataCreator<VERTEX_TYPE, INDEX_TYPE>::CreateDxMeshData(
		ID3D11Device* _poDevice, 
		const std::vector<VERTEX_TYPE>& vertices, 
		const std::vector<INDEX_TYPE>& indices)
	{
		m_meshData.originalMeshData.vertices = vertices;
		m_meshData.originalMeshData.indices = indices;

		FillVertexBuffer(_poDevice, vertices.size(), vertices.data());
		FillIndexBuffer(_poDevice, indices.size(), indices.data());

		m_meshData.indexCount = static_cast<uint32_t>(indices.size());
		m_meshData.indexFormat = DXGI_FORMAT_R32_UINT;
		m_meshData.enumTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		return m_meshData;
	}

	template<typename VERTEX_TYPE, typename INDEX_TYPE>
	inline bool DxMeshDataCreator<VERTEX_TYPE, INDEX_TYPE>::FillIndexBuffer(ID3D11Device* _poDevice, uint32_t indexCount, const INDEX_TYPE* pIndices)
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
		HRESULT hr = _poDevice->CreateBuffer(&vertexBufferDesc, &indexData, &m_meshData.cpoIndexBuffer);
		assert(SUCCEEDED(hr));

		return SUCCEEDED(hr);
	}

	template<typename VERTEX_TYPE, typename INDEX_TYPE>
	inline bool DxMeshDataCreator<VERTEX_TYPE, INDEX_TYPE>::FillVertexBuffer(ID3D11Device* _poDevice, uint32_t vertexCount, const VERTEX_TYPE* pVertices)
	{
		if (vertexCount == 0)
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
		vertexData.pSysMem = pVertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		HRESULT hr = _poDevice->CreateBuffer(&vertexBufferDesc, (vertexData.pSysMem) ? &vertexData : NULL, &m_meshData.cpoVertexBuffer);
		assert(SUCCEEDED(hr));

		return SUCCEEDED(hr);
	}
	// TODO: remove once tested
	static void DEBUG_TestInitMethod(ID3D11Device* poDevice)
	{
		DirectX::DX11::GeometricPrimitive::VertexCollection vertices;
		DirectX::DX11::GeometricPrimitive::IndexCollection indices;
		DirectX::DX11::GeometricPrimitive::CreateCube(vertices, indices);

		TRawMeshData<CommonVertex, uint32_t> rawMeshData;

		for (size_t iVertex = 0; iVertex < vertices.size(); iVertex++)
		{
			auto& inVertex = vertices[iVertex];
			CommonVertex commonVertex;
			commonVertex.position = { inVertex.position.x, inVertex.position.y, inVertex.position.z, 1 };
			commonVertex.normal = inVertex.normal;
			commonVertex.textureCoordinate = inVertex.textureCoordinate;

			rawMeshData.vertices.push_back(commonVertex);
		}

		for (size_t iIndex = 0; iIndex < indices.size(); iIndex++) {			
			rawMeshData.indices.push_back(indices[iIndex]);
		}
		auto meshCreator = DxMeshDataCreator<CommonVertex, uint32_t>();
		auto result = meshCreator.CreateDxMeshData(poDevice, rawMeshData.vertices, rawMeshData.indices);

		auto debug_break = 1;
	}
};
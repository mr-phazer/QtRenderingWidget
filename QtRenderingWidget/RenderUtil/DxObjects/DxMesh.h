#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <typeinfo>
#include <cassert>

#include "..\..\..\DirectXTK\Inc\GeometricPrimitive.h"
#include "..\..\..\DirectXTK\Inc\VertexTypes.h"
#include "..\Managers\DxResourceManager.h"

#include "IDrawable.h"
#include "DxShaderProgram.h"
#include "..\Types\CommonVertexType.h"

using namespace DirectX;


namespace Rldx {

	class DxMeshData
	{
	public:	

		uint32_t                                                indexCount = 0;
		uint32_t                                                startIndex = 0;
		int32_t                                                 vertexOffset = 0;
		uint32_t                                                vertexStride = sizeof(CommonVertex);
		D3D_PRIMITIVE_TOPOLOGY                                  primitiveType;
		DXGI_FORMAT                                             indexFormat;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>               m_cpoInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>					m_cpoIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>					m_cpoVertexBuffer;
		DXGI_FORMAT m_enumIndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
		D3D11_PRIMITIVE_TOPOLOGY m_enumTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	};

	class DxMesh : public IDrawable
	{
		DxMeshData* m_meshData;

	public:
		void Draw(ID3D11DeviceContext* poDC, IDxShaderProgram* shaderProgram = nullptr, ID3D11RenderTargetView* destRtV = nullptr) override
		{	
			// if RTV is supplied set that as active
			if (destRtV != nullptr) {
				poDC->OMSetRenderTargets(1, &destRtV, nullptr);
			}

			GetMeshReadyForDrawing(poDC);

			poDC->DrawIndexed(m_meshData->indexCount, 0, 0);
		};

		virtual void GetMeshReadyForDrawing(ID3D11DeviceContext* poDC)
		{
			UINT stride = sizeof(CommonVertex);
			UINT offset = 0;

			poDC->IASetVertexBuffers(0, 1, m_meshData->m_cpoVertexBuffer.GetAddressOf(), &stride, &offset);
			poDC->IASetIndexBuffer(m_meshData->m_cpoIndexBuffer.Get(), m_meshData->m_enumIndexFormat, 0);

			poDC->IASetPrimitiveTopology(m_meshData->m_enumTopology);
		};

		void SetMeshData(const DxMeshData& meshData)
		{
			m_meshData = DxResourceManager::GetInstance().GetMeshes().AddResource(meshData).GetPtr();
		};		
	};


	template <typename INDEX_TYPE, typename VERTEX_TYPE>
	class DxMeshCreator
	{
		DxMeshData CreateMesh(ID3D11Device* _poDevice, std::vector<VERTEX_TYPE>& vertices, std::vector<INDEX_TYPE>& indices);

	private:
		bool FillIndexBuffer(ID3D11Device* _poDevice, uint32_t indexCount, const INDEX_TYPE* pIndices);
		bool FillVertexBuffer(ID3D11Device* _poDevice, uint32_t vertexCount, const VERTEX_TYPE* pVertices);
	private:
		DxMeshData m_meshData;

	}; // class DxMeshCreator

	template<typename INDEX_TYPE, typename VERTEX_TYPE>
	inline DxMeshData DxMeshCreator<INDEX_TYPE, VERTEX_TYPE>::CreateMesh(ID3D11Device* _poDevice, std::vector<VERTEX_TYPE>& vertices, std::vector<INDEX_TYPE>& indices)
	{
		FillIndexBuffer(_poDevice, indices.size(), indices.data());
		FillVertexBuffer(_poDevice, vertices.size(), vertices.data());

		m_meshData.indexCount = static_cast<uint32_t>(indices.size());
		m_meshData.indexFormat = DXGI_FORMAT_R16_UINT;
		m_meshData.primitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		return m_meshData;
	}

	template<typename INDEX_TYPE, typename VERTEX_TYPE>
	inline bool DxMeshCreator<INDEX_TYPE, VERTEX_TYPE>::FillIndexBuffer(ID3D11Device* _poDevice, uint32_t indexCount, const INDEX_TYPE* pIndices)
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
		HRESULT hr = _poDevice->CreateBuffer(&vertexBufferDesc, &indexData, &m_meshData.m_cpoIndexBuffer);
		assert(SUCCEEDED(hr));

		return m_cpoIndexBuffer;

	}

	template<typename INDEX_TYPE, typename VERTEX_TYPE>
	inline bool DxMeshCreator<INDEX_TYPE, VERTEX_TYPE>::FillVertexBuffer(ID3D11Device* _poDevice, uint32_t vertexCount, const VERTEX_TYPE* pVertices)
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
		HRESULT hr = _poDevice->CreateBuffer(&vertexBufferDesc, (vertexData.pSysMem) ? &vertexData : NULL, &m_meshData.m_cpoVertexBuffer);
		assert(SUCCEEDED(hr));

		return SUCCEEDED(hr);
	}

class MeshRenderer
{
public:
	void Draw(ID3D11DeviceContext* poDC, IDxShaderProgram* shaderProgram, IDxMaterial* material);
	
private:
	void GetMaterialReady(ID3D11DeviceContext* poDC, IDxMaterial* material);
	void GetShadersReady(ID3D11DeviceContext* poDC, IDxShaderProgram* shaderProgram);
	void GetMeshReadyForDrawing(ID3D11DeviceContext* poDC);	
}

}; // namespace Rldx


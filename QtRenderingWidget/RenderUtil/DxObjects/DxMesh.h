#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <typeinfo>
#include <cassert>

#include "..\..\..\DirectXTK\Inc\GeometricPrimitive.h"
#include "..\..\..\DirectXTK\Inc\VertexTypes.h"

#include "IDrawable.h"

using namespace DirectX;


namespace Rldx {

	struct DxMeshData
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_cpoIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_cpoVertexBuffer;
	};	

	class DxMesh : public IDrawable
	{
	public:
		void Draw(ID3D11DeviceContext* poDeviceContext, IDxShaderProgram* shaderProgram = nullptr, ID3D11RenderTargetView* destRtV = nullptr) override
		{
			// TODO: finish, draw mesh
			
			poDeviceContext->OMSetRenderTargets(1, &destRtV, nullptr);
			/*
				- From mesh.material:
					- set shader program - if any, else the shader program passed in this method
					  - set PS shaders
					  - set VS shaders
					  - update VS const buffers
					  - update PS const buffers
					- set textures (SRV) if existing - else default textures					

				- set vertex buffer
				- set index buffer				
				- D3ddevice.drawindexed()			
			*/			
		}

		void SetMeshData(const DxMeshData& meshData)
		{
			m_meshData = meshData;
		};

	private:
		DxMeshData m_meshData;
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

		return m_tempMeshData;
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

}; // namespace Rldx


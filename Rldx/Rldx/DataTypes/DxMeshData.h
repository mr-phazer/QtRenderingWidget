#pragma once

#include <d3d11.h>
#include <vector>
#include <wrl\client.h>
#include "..\DataTypes\CommonVertex.h"

namespace rldx
{
	/// <summary>
	/// Template for storing a general CPU-side mesh
	/// </summary>
	/// <typeparam m_nodeName="VERTEX_TYPE">Vertex type</typeparam>
	/// <typeparam m_nodeName="INDEX_TYPE">has to 2 or 4 byte uint</typeparam>
	template <typename VERTEX_TYPE, typename INDEX_TYPE>
	struct TRawMeshData
	{
		std::vector<VERTEX_TYPE> vertices;
		std::vector<INDEX_TYPE> indices;
	};

	/// <summary>
	/// For holding a general GPU-side mesh data, plus copy of original (for CPU-write access DX mesh buffers)
	/// </summary>
	/// <typeparam m_nodeName="VertexType"></typeparam>
	/// <typeparam m_nodeName="IndexType"></typeparam>
	template <typename VertexType, typename IndexType>
	struct TDxMeshRenderData
	{
		uint32_t								indexCount = 0;
		uint32_t								startIndex = 0;
		int32_t									vertexOffset = 0;
		uint32_t								vertexStride = sizeof(VertexType);
		DXGI_FORMAT								indexFormat = sizeof(IndexType) == 2 ? DXGI_FORMAT::DXGI_FORMAT_R16_UINT : DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		D3D11_PRIMITIVE_TOPOLOGY				primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		Microsoft::WRL::ComPtr<ID3D11Buffer>	cpoIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>	cpoVertexBuffer;

		DirectX::XMFLOAT3 min, max;
		TRawMeshData<VertexType, IndexType>	originalMeshData;
	};

	using DxCommonMeshData = TDxMeshRenderData<CommonVertex, uint32_t>;
};
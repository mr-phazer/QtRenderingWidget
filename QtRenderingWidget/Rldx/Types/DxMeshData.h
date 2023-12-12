#pragma once

#include <vector>
#include <d3d11.h>
#include <wrl\client.h>
#include "..\Types\CommonVertexType.h"

namespace Rldx {

	template <typename VERTEX_TYPE, typename INDEX_TYPE>
	struct TRawMeshData
	{
		std::vector<VERTEX_TYPE> vertices;
		std::vector<INDEX_TYPE> indices;
	};


	template <typename VERTEX_TYPE, typename INDEX_TYPE>
	struct TDxMeshData
	{
		~TDxMeshData() = default;

		uint32_t                                                indexCount = 0;
		uint32_t                                                startIndex = 0;
		int32_t                                                 vertexOffset = 0;
		uint32_t                                                vertexStride = sizeof(VERTEX_TYPE);
		DXGI_FORMAT                                             indexFormat;
		DXGI_FORMAT												enumIndexFormat = sizeof(INDEX_TYPE) == 2 ? DXGI_FORMAT::DXGI_FORMAT_R16_UINT : DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		D3D11_PRIMITIVE_TOPOLOGY								enumTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>               cpoInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>					cpoIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>					cpoVertexBuffer;

		TRawMeshData<VERTEX_TYPE, INDEX_TYPE> originalMeshData;
	};

	using DxCommonMeshData = TDxMeshData<CommonVertex, uint32_t>;
};
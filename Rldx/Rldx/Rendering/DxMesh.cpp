#include "DxMesh.h"

namespace rldx
{
	void DxMesh::Draw(ID3D11DeviceContext* poDC)
	{
		poDC->DrawIndexed(m_poMeshBuffers.indexCount, 0, 0);
	}

	ResourceTypeEnum DxMesh::GetType() const
	{
		return ResourceTypeEnum::MeshRenderData;
	}

	std::wstring DxMesh::GetTypeString() const
	{
		return L"DxMesh";
	}

	DirectX::XMFLOAT3 DxMesh::GetMin()
	{
		DirectX::XMFLOAT3 v3Min(FLT_MAX, FLT_MAX, FLT_MAX);
		for (const auto& vertex : m_poMeshBuffers.originalMeshData.vertices)
		{
			v3Min.x = min<float>(v3Min.x, vertex.position.x);
			v3Min.y = min<float>(v3Min.y, vertex.position.y);
			v3Min.z = min<float>(v3Min.z, vertex.position.z);
		}

		return v3Min;
	}

	DirectX::XMFLOAT3 DxMesh::GetMax()
	{
		DirectX::XMFLOAT3 v3Max(FLT_MIN, FLT_MIN, FLT_MIN);
		for (const auto& vertex : m_poMeshBuffers.originalMeshData.vertices)
		{
			v3Max.x = max<float>(v3Max.x, vertex.position.x);
			v3Max.y = max<float>(v3Max.y, vertex.position.y);
			v3Max.z = max<float>(v3Max.z, vertex.position.z);
		}

		return v3Max;
	}

	void DxMesh::SetMeshData(const DxCommonMeshData& meshData)
	{
		m_poMeshBuffers = meshData;
	}

	void DxMesh::BindToDC(ID3D11DeviceContext* poDC)
	{
		UINT stride = sizeof(CommonVertex);
		UINT offset = 0;
		poDC->IASetVertexBuffers(0, 1, m_poMeshBuffers.cpoVertexBuffer.GetAddressOf(), &stride, &offset);
		poDC->IASetIndexBuffer(m_poMeshBuffers.cpoIndexBuffer.Get(), m_poMeshBuffers.indexFormat, 0);
		poDC->IASetPrimitiveTopology(m_poMeshBuffers.primitiveTopology);
	}
}

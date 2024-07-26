#include "DxMesh.h"

namespace rldx
{
	void DxMesh::Draw(ID3D11DeviceContext* poDC)
	{
		poDC->DrawIndexed(m_poMeshBuffers.indexCount, 0, 0);
	}

	const DirectX::XMFLOAT3& DxMesh::GetMin() const
	{
		return m_poMeshBuffers.min;
	}

	const DirectX::XMFLOAT3& DxMesh::GetMax() const
	{
		return m_poMeshBuffers.max;
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

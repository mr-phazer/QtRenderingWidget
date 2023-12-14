#include "DxMesh.h"

using namespace Rldx;

ResourceTypeEnum Rldx::DxMesh::GetType() const
{
    return ResourceTypeEnum::Mesh;
}

void Rldx::DxMesh::SetMeshData(const DxCommonMeshData& meshData)
{
	m_meshData = meshData;
}

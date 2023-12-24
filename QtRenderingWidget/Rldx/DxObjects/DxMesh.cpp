#include "DxMesh.h"

using namespace Rldx;

ResourceTypeEnum Rldx::DxMesh::GetType() const
{
	return ResourceTypeEnum::MeshData;
}

std::string Rldx::DxMesh::GetTypeString() const
{
	return "DxMesh";
}

void Rldx::DxMesh::SetMeshData(const DxCommonMeshData& meshData)
{
	m_meshData = meshData;
}

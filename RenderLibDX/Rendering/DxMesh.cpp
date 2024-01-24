#include "DxMesh.h"

using namespace rldx;

ResourceTypeEnum rldx::DxMesh::GetType() const
{
	return ResourceTypeEnum::MeshData;
}

std::string rldx::DxMesh::GetTypeString() const
{
	return "DxMesh";
}

void rldx::DxMesh::SetMeshData(const DxCommonMeshData& meshData)
{
	m_poMesh = meshData;
}

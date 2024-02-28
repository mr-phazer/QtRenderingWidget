#include "DxMesh.h"

using namespace rldx;

ResourceTypeEnum rldx::DxMesh::GetType() const
{
	return ResourceTypeEnum::MeshData;
}

std::wstring rldx::DxMesh::GetTypeString() const
{
	return L"DxMesh";
}

void rldx::DxMesh::SetMeshData(const DxCommonMeshData& meshData)
{
	m_poMesh = meshData;
}

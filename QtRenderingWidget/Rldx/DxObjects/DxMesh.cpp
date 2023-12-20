#include "DxMesh.h"

using namespace Rldx;

SceneNodeTypeEnum Rldx::DxMesh::GetType() const
{
    return SceneNodeTypeEnum::Mesh;
}

void Rldx::DxMesh::SetMeshData(const DxCommonMeshData& meshData)
{
	m_meshData = meshData;
}

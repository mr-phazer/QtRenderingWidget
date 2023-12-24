#include "DxMeshNode.h"

Rldx::DxMeshNode::Sptr Rldx::DxMeshNode::Create(const std::string& name)
{
	auto newMeshNode = std::shared_ptr<DxMeshNode>(new DxMeshNode(name));

	return newMeshNode;
}

void Rldx::DxMeshNode::Draw(ID3D11DeviceContext* poDC)
{
	// ready shader program
	m_shaderProgram->GetReady(poDC);

	// draw mesh
	m_meshData->Draw(poDC);
}


// TODO: clean up includes

#include "..\..\DataTypes\DxMeshData.h"

#include "..\..\Interfaces\IRenderBucket.h"
#include "..\..\Rendering\DxMesh.h"
#include "DxMeshNode.h"
#include "..\..\Rendering\DxMeshRenderData.h"
//#include "..\SceneNodes\DxBaseNode.h"



using namespace rldx;

rldx::DxMeshNode::SharedPtr rldx::DxMeshNode::Create(const std::string& name)
{
	auto newMeshNode = std::shared_ptr<DxMeshNode>(new DxMeshNode);
	newMeshNode->SetName(name);
	return newMeshNode;
}

void rldx::DxMeshNode::SetModelData(const rldx::DxCommonMeshData& meshData)
{
	auto newMeshHandle = DxResourceManager::Instance()->AllocMesh();
	m_meshData.m_poMesh = newMeshHandle.GetPtr();
	m_meshData.m_poMesh->SetMeshData(meshData);

	// TODO: REMOVE:
	auto DEBUG_BREAK = 1;

}

void rldx::DxMeshNode::SetBoundingBox(DirectX::XMFLOAT3 minPoint, DirectX::XMFLOAT3 maxPoint)
{
	DirectX::XMVECTOR xmMin = DirectX::XMLoadFloat3(&minPoint);
	DirectX::XMVECTOR xmMax = DirectX::XMLoadFloat3(&maxPoint);

	DirectX::BoundingBox::CreateFromPoints(m_meshData.m_BB, xmMin, xmMax);	
}

//void rldx::DxMeshNode::Draw(ID3D11DeviceContext* poDC)
//{
//	// ready shader program
//	m_poShaderProgram->GetReady(poDC);
//
//	// draw mesh
//	m_poMesh->Draw(poDC);
//}


void DxMeshNode::FlushToRenderBucket(IRenderBucket* pRenderQueue)
{
	if (!GetDrawState()) {
		return;
	}

	m_meshData.m_mWorldMatrix = GetCurrentGlobalTransForm();
	m_meshData.m_pivotPoint = { 0.0f, 0.0f, 0.0f }; // TODO: actually set pivot point from the node, not just 0,0,0

	pRenderQueue->AddItem(&m_meshData);
}

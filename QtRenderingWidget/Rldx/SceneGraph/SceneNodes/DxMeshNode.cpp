#include "..\..\DataTypes\DxMeshData.h"

#include "..\..\Interfaces\IRenderBucket.h"
#include "..\..\Rendering\DxMesh.h"
#include "DxMeshNode.h"
#include "..\..\Rendering\DxMeshDrawable.h"
#include "..\BaseNode\DxBaseNode.h"



using namespace Rldx;

Rldx::DxMeshNode::SharedPointer Rldx::DxMeshNode::Create(const std::string& name)
{
	auto newMeshNode = std::shared_ptr<DxMeshNode>(new DxMeshNode);
	newMeshNode->SetName(name);
	return newMeshNode;
}

void Rldx::DxMeshNode::SetMeshData(const Rldx::DxCommonMeshData& meshData)
{
	auto newMeshHandle = DxResourceManager::Instance()->AllocMesh();
	m_meshDrawable.m_poMesh = newMeshHandle.GetPtr();
	m_meshDrawable.m_poMesh->SetMeshData(meshData);

	// TODO: REMOVE:
	auto DEBUG_BREAK = 1;

}

//void Rldx::DxMeshNode::Draw(ID3D11DeviceContext* poDC)
//{
//	// ready shader program
//	m_poShaderProgram->GetReady(poDC);
//
//	// draw mesh
//	m_poMesh->Draw(poDC);
//}

void Rldx::DxMeshNode::Update(float timeElapsed)
{
	UppdateGlobalTransform(timeElapsed);
}

void DxMeshNode::FlushToRenderQueue(IRenderBucket* pRenderQueue)
{
	m_meshDrawable.m_mWorldMatrix = GetCurrentGlobalTransForm();
	m_meshDrawable.m_pivotPoint = { 0.0f, 0.0f, 0.0f }; // TODO: actually set pivot point from the node, not just 0,0,0

	pRenderQueue->AddItem(&m_meshDrawable);
}

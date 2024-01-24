// TODO: clean up includes

#include "..\..\DataTypes\DxMeshData.h"

#include "..\..\Interfaces\IRenderBucket.h"
#include "..\..\Rendering\DxMesh.h"
#include "DxMeshNode.h"
#include "..\..\Rendering\DxMeshDrawable.h"
//#include "..\SceneNodes\DxBaseNode.h"



using namespace rldx;

rldx::DxMeshNode::SharedPointer rldx::DxMeshNode::Create(const std::string& name)
{
	auto newMeshNode = std::shared_ptr<DxMeshNode>(new DxMeshNode);
	newMeshNode->SetName(name);
	return newMeshNode;
}

void rldx::DxMeshNode::SetMeshData(const rldx::DxCommonMeshData& meshData)
{
	auto newMeshHandle = DxResourceManager::Instance()->AllocMesh();
	m_meshDrawable.m_poMesh = newMeshHandle.GetPtr();
	m_meshDrawable.m_poMesh->SetMeshData(meshData);

	// TODO: REMOVE:
	auto DEBUG_BREAK = 1;

}

//void rldx::DxMeshNode::Draw(ID3D11DeviceContext* poDC)
//{
//	// ready shader program
//	m_poShaderProgram->GetReady(poDC);
//
//	// draw mesh
//	m_poMesh->Draw(poDC);
//}


void DxMeshNode::FlushToRenderQueue(IRenderBucket* pRenderQueue)
{
	m_meshDrawable.m_mWorldMatrix = GetCurrentGlobalTransForm();
	m_meshDrawable.m_pivotPoint = { 0.0f, 0.0f, 0.0f }; // TODO: actually set pivot point from the node, not just 0,0,0

	pRenderQueue->AddItem(&m_meshDrawable);
}

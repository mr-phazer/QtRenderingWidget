#include "DxMeshNode.h"
#include "..\Rldx\Interfaces\IRenderBucket.h"

using namespace Rldx;

DxMeshNode::SharedPointerMesh DxMeshNode::Create(const std::string& name)
{
	auto newMeshNode = std::shared_ptr<DxMeshNode>(new DxMeshNode);
	newMeshNode->SetName(name);
	return newMeshNode;
}

//void Rldx::DxMeshNode::Draw(ID3D11DeviceContext* poDC)
//{
//	// ready shader program
//	m_shaderProgram->GetReady(poDC);
//
//	// draw mesh
//	m_meshData->Draw(poDC);
//}

void Rldx::DxMeshNode::FlushToRenderQueue(IRenderBucket* pRenderQueue)
{	
	sm::Matrix mWorld = GetCurrentGlobalTransForm();

	DxRenderItemMesh newRenderQueueItem(
		m_meshData, 
		m_shaderProgram, 
		m_material, 
		(m_pDeformerNode) ? &m_pDeformerNode->m_deformerDataBuffer : nullptr);
	
	pRenderQueue->AddItem(newRenderQueueItem);
}

void Rldx::DxMeshNode::Update(float timeElapsed)
{
	UppdateGlobalTransform(timeElapsed);
}


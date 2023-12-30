#include "DxMeshNode.h"
#include "..\Interfaces\IRenderQueue.h"

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

void Rldx::DxMeshNode::FlushToRenderQueue(IDxRenderQueue* pRenderQueue)
{	
	sm::Matrix mWorld = GetCurrentGlobalTransForm();

	DxRenderQueueItem newRenderQueueItem;
	newRenderQueueItem.meshData = m_meshData;
	newRenderQueueItem.shaderProgram = m_shaderProgram;
	newRenderQueueItem.material = m_material;
	newRenderQueueItem.worldMatrix = mWorld;
	newRenderQueueItem.pivotPoint = { 0,0,0 };

	pRenderQueue->AddItem(newRenderQueueItem);
}

void Rldx::DxMeshNode::Update(float timeElapsed)
{
	UppdateGlobalTransform(timeElapsed);
}


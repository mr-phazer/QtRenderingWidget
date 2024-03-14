// TODO: clean up includes

#include "..\..\DataTypes\DxMeshData.h"

#include "..\..\Interfaces\IRenderBucket.h"
#include "..\..\Rendering\DxMesh.h"
#include "..\..\Rendering\DxMeshRenderData.h"
#include "DxMeshNode.h"
//#include "..\SceneNodes\DxBaseNode.h"



namespace rldx
{
	DxMeshNode::SharedPtr DxMeshNode::Create(const std::wstring& name)
	{
		auto newMeshNode = std::shared_ptr<DxMeshNode>(new DxMeshNode);
		newMeshNode->SetName(name);
		return newMeshNode;
	}

	void DxMeshNode::SetModelData(const DxCommonMeshData& meshData)
	{
		auto newMeshHandle = DxResourceManager::Instance()->AllocMesh();
		m_meshData.m_poMesh = newMeshHandle.GetPtr();
		m_meshData.m_poMesh->SetMeshData(meshData);
	}

	void DxMeshNode::SetBoundingBox(DirectX::XMFLOAT3 minPoint, DirectX::XMFLOAT3 maxPoint)
	{
		DirectX::XMVECTOR xmMin = DirectX::XMLoadFloat3(&minPoint);
		DirectX::XMVECTOR xmMax = DirectX::XMLoadFloat3(&maxPoint);

		DirectX::BoundingBox::CreateFromPoints(GetNodeBoundingBox(), xmMin, xmMax);
	}

	//void DxMeshNode::Draw(ID3D11DeviceContext* poDC)
	//{
	//	// ready shader program
	//	m_poShaderProgram->GetReady(poDC);
	//
	//	// draw mesh
	//	m_poMesh->Draw(poDC);
	//}


	void DxMeshNode::FlushToRenderBucket(IRenderBucket* pRenderQueue)
	{
		if (GetDrawState() != DrawStateEnum::Draw) {
			return;
		}

		m_meshData.m_mWorldMatrix = GetCurrentGlobalTransForm();
		m_meshData.m_pivotPoint = { 0.0f, 0.0f, 0.0f }; // TODO: actually set pivot point from the node, not just 0,0,0

		pRenderQueue->AddItem(&m_meshData);
	}
}
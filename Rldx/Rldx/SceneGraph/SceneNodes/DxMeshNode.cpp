// TODO: clean up includes

#include "..\..\DataTypes\DxMeshData.h"
#include "..\..\Managers\DxDeviceManager.h"

#include "..\..\Interfaces\IRenderBucket.h"
#include "..\..\Rendering\DxMesh.h"

#include "DxMeshNode.h"


namespace rldx
{

	DxMeshNode::SharedPtr DxMeshNode::Create(const std::wstring& name)
	{
		auto newMeshNode = std::shared_ptr<DxMeshNode>(new DxMeshNode);
		newMeshNode->SetName(name);

		newMeshNode->m_meshData.CreateConstBuffers(DxDeviceManager::Device());
		return newMeshNode;
	}

	void DxMeshNode::SetMeshData(const DxCommonMeshData& meshData, std::wstring meshName, sm::Matrix mWeaponMatrix)
	{
		auto newMeshHandle = DxResourceManager::Instance()->AllocMesh();
		m_meshData.poMesh = newMeshHandle.GetPtr();
		m_meshData.poMesh->SetMeshData(meshData);
		m_meshData.meshName = meshName;
	}

	void DxMeshNode::SetDeformerNode(const rldx::DxDeformerNode* poDeformerNode, int32_t boneIndex)
	{
		m_meshData.poDeformerNode = poDeformerNode;
		m_meshData.attachPoint.boneIndex = boneIndex;
	}

	void DxMeshNode::SetAttachBoneAsParent()
	{
		if (GetParent())
		{
			m_meshData.attachPoint.boneIndex = static_cast<DxMeshNode*>(GetParent())->m_meshData.attachPoint.boneIndex;
		}

	}

	void DxMeshNode::SetShaderProgram(DxMeshShaderProgram* shaderProgram) { m_meshData.poShaderProgram = shaderProgram; }

	void DxMeshNode::SetBoundingBox(DirectX::XMFLOAT3 minPoint, DirectX::XMFLOAT3 maxPoint)
	{
		DirectX::XMVECTOR xmMin = DirectX::XMLoadFloat3(&minPoint);
		DirectX::XMVECTOR xmMax = DirectX::XMLoadFloat3(&maxPoint);

		DirectX::BoundingBox::CreateFromPoints(GetNodeBoundingBox(), xmMin, xmMax);
	}

	void DxMeshNode::SetBoundingBox(const DirectX::BoundingBox& inBB)
	{
		GetNodeBoundingBox() = inBB;
	}

	void DxMeshNode::FlushToRenderBucket(IRenderBucket* pRenderQueue)
	{
		if (GetDrawState() != DrawStateEnum::Draw) {
			return;
		}

		// copy node geomtry to VS const buffer
		m_meshData.perMesh_VS_CB.data.mWorld = GetCurrentGlobalTransForm();
		m_meshData.perMesh_VS_CB.data.pivot = { 0.0f, 0.0f, 0.0f }; // TODO: actually set pivot point from the node, not just 0,0,0

		pRenderQueue->AddItem(&m_meshData);
	}

	void DxMeshNode::Update(float time)
	{
		DxBaseNode::Update(time);

	}
}
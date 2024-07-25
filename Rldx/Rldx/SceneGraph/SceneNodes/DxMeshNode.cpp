// TODO: clean up includes

#include "..\..\DataTypes\DxMeshData.h"
#include "..\..\Interfaces\IRenderBucket.h"
#include "..\..\Managers\DxDeviceManager.h"
#include "..\..\Rendering\DxMesh.h"
#include "..\..\Rendering\DxShaderProgram.h"
#include "DxMeshNode.h"

namespace rldx
{
	DxMeshNode::UniquePtr DxMeshNode::Create(const std::wstring& name)
	{
		auto newMeshNode = std::make_unique<DxMeshNode>(name);
		return std::move(newMeshNode);
	}

	// TODO: is needed? Remove?
	void DxMeshNode::Clone(DxMeshRenderingData& clone) const {
		//DxMeshRenderingData clone;

		clone.meshName = m_meshData.meshName;
		clone.poMesh = m_meshData.poMesh;
		clone.poMaterial = m_meshData.poMaterial;
		clone.poShaderProgram = m_meshData.poShaderProgram;
		clone.poDeformerSourceNode = m_meshData.poDeformerSourceNode;
		clone.pivot = m_meshData.pivot;

		clone.attachPoint = m_meshData.attachPoint;

		clone.perMesh_VS_CB.data = m_meshData.perMesh_VS_CB.data;
		clone.perMesh_PS_CB.data = m_meshData.perMesh_PS_CB.data;
		clone.perMeshDerformer_VS_CB.data = m_meshData.perMeshDerformer_VS_CB.data;
	}

	void DxMeshNode::SetMeshData(DxResourceManager& resourceManager, const DxCommonMeshData& meshData, std::wstring meshName, sm::Matrix mWeaponMatrix)
	{
		auto newMeshHandle = resourceManager.CreateResouce<DxMesh>();
		m_meshData.poMesh = newMeshHandle;
		m_meshData.poMesh->SetMeshData(meshData);
		m_meshData.meshName = meshName;

		DxMeshNode::ResizeBoundBoxToContent();
	}

	void DxMeshNode::ResizeBoundBoxToContent()
	{
		// Make node bound box that fits the new mesh data
		DirectX::BoundingBox::CreateFromPoints(
			NodeBoundingBox(),
			DirectX::XMLoadFloat3(&m_meshData.poMesh->GetMin()),
			DirectX::XMLoadFloat3(&m_meshData.poMesh->GetMax())
		);
	}

	void DxMeshNode::SetDeformerNode(const rldx::DxDeformerNode* poDeformerNode, int32_t boneIndex)
	{
		m_meshData.poDeformerSourceNode = poDeformerNode;
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


	void DxMeshNode::FlushToRenderBucket(IRenderBucket* pRenderQueue)
	{
		if (GetDrawState() == DrawStateEnum::Draw)
		{
			DxBaseNode::FlushToRenderBucket(pRenderQueue);
			pRenderQueue->AddItem(&m_meshData);
		}
	}

	void DxMeshNode::Update(float time)
	{
		DxBaseNode::Update(time);

		UpdateGlobalTransform();
		auto mParentWorld = GetCurrentGlobalTransForm().Transpose();
		SetNodeWorldTransForm(mParentWorld);
	}
}
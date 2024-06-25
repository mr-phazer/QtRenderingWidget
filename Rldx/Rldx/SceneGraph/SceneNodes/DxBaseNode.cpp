#include "..\..\Interfaces\IRenderBucket.h"
#include "..\..\Rendering\DxMesh.h"
#include "..\Rldx\Rldx\Helpers\DxMeshCreatorHelper.h"
#include "..\Rldx\Rldx\Managers\DxDeviceManager.h"
#include "..\Rldx\Rldx\Rendering\DxShaderProgram.h"
#include "DxBaseNode.h"


namespace rldx
{
	std::wstring DxBaseNode::GetTypeString() const
	{
		return L"DxBaseNode";
	}

	SceneNodeTypeEnum DxBaseNode::GetType() const
	{
		return SceneNodeTypeEnum::EmptyNode;
	}

	void DxBaseNode::SetBoundingBox(const DirectX::BoundingBox& inBB)
	{
		GetNodeBoundingBox() = inBB;

		auto newSimpleShaderProgram =
			rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
				rldx::DxDeviceManager::Device(),
				LR"(VS_Simple.cso)",
				LR"(PS_Simple.cso)"
			);

		auto bbdMeshData = rldx::DxMeshCreatorHelper::MakeBoundingBoxMesh(rldx::DxDeviceManager::Device(), GetNodeBoundingBox());

		auto newMeshHandle = DxResourceManager::Instance()->AllocMesh();
		m_boundingBoxMesh.poMesh = newMeshHandle.GetPtr();
		m_boundingBoxMesh.poMesh->SetMeshData(bbdMeshData);
		//m_boundingBoxMesh.poShaderProgram = newSimpleShaderProgram;
		m_boundingBoxMesh.poShaderProgram =
			m_boundingBoxMesh.poShaderProgram = DefaultShaderCreator::GetSimpleShaderProgram();
		m_boundingBoxMesh.meshName = L"BoundNox";
		m_boundingBoxMesh.CreateConstBuffers_DOES_NOTHING__REMOVE(rldx::DxDeviceManager::Device());
	}

	void DxBaseNode::SetBoundingBox(DirectX::XMFLOAT3 minPoint, DirectX::XMFLOAT3 maxPoint)
	{
		DirectX::XMVECTOR xmMin = DirectX::XMLoadFloat3(&minPoint);
		DirectX::XMVECTOR xmMax = DirectX::XMLoadFloat3(&maxPoint);

		DirectX::BoundingBox::CreateFromPoints(GetNodeBoundingBox(), xmMin, xmMax);

		auto newSimpleShaderProgram =
			rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
				rldx::DxDeviceManager::Device(),
				LR"(VS_Simple.cso)",
				LR"(PS_Simple.cso)"
			);

		auto bbdMeshData = rldx::DxMeshCreatorHelper::MakeBoundingBoxMesh(rldx::DxDeviceManager::Device(), GetNodeBoundingBox());

		auto newMeshHandle = DxResourceManager::Instance()->AllocMesh();
		m_boundingBoxMesh.poMesh = newMeshHandle.GetPtr();
		m_boundingBoxMesh.poMesh->SetMeshData(bbdMeshData);
		//m_boundingBoxMesh.poShaderProgram = newSimpleShaderProgram;
		m_boundingBoxMesh.poShaderProgram = DefaultShaderCreator::GetSimpleShaderProgram();
		m_boundingBoxMesh.meshName = L"BoundNox";
		m_boundingBoxMesh.CreateConstBuffers_DOES_NOTHING__REMOVE(rldx::DxDeviceManager::Device());
	}

	void DxBaseNode::SetDeformerNodeRecursive(const DxDeformerNode* poDeformerNode, int32_t boneIndex)
	{
		SetDeformerNode(poDeformerNode, boneIndex);

		for (auto& itChild : GetChildren())
		{
			itChild->SetDeformerNodeRecursive(poDeformerNode, boneIndex);
		}
	}

	void DxBaseNode::Update(float timeElapsed)
	{
		UpdateGlobalTransform(timeElapsed);

		for (const char it : "somestring")
		{
			auto test = it;
		}
	}

	void DxBaseNode::FlushToRenderBucket(IRenderBucket* pRenderQueue)
	{
		if (m_boundingBoxMesh.poMesh && m_BoundingBoxDrawState == DrawStateEnum::Draw)
		{
			pRenderQueue->AddItem(&m_boundingBoxMesh);
		}

		// TODO: add small cube to renderqueue, cube for specifying node translation
		// TODO: draw BoundBox
	}

} // namespace rldx

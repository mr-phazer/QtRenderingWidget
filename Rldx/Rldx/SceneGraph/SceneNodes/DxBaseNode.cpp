#include <CommonLibs\CustomExceptions\CustomExceptions.h>
#include "..\..\Interfaces\IRenderBucket.h"
#include "..\..\Rendering\DxMesh.h"
#include <Rldx\Rldx\Helpers\DxMeshCreatorHelper.h>
#include <Rldx\Rldx\Managers\DxDeviceManager.h>
#include <Rldx\Rldx\Rendering\DxShaderProgram.h>
#include "DxBaseNode.h"


namespace rldx
{
	void DxBaseNode::AllocateBoundingBoxMesh(DxBaseNode* node, DxResourceManager& resourceManager)
	{
		if (!node) throw COMException(L"input node == nullptr");

		auto bbdMeshData = rldx::DxMeshCreatorHelper::MakeBoundingBoxMesh(rldx::DxDeviceManager::Device(), node->NodeBoundingBox(), node->boundBoxRenderColor);

		auto newMeshHandle = resourceManager.CreateResouce<DxMesh>();

		node->m_boundingBoxMesh.poMesh = newMeshHandle;
		node->m_boundingBoxMesh.poMesh->SetMeshData(bbdMeshData);

		node->m_boundingBoxMesh.poShaderProgram = DefaultShaderCreator::GetSimpleShaderProgram(resourceManager);
		node->m_boundingBoxMesh.meshName = L"Mesh:BoundBox";
	}

	void DxBaseNode::SetDeformerNode(const rldx::DxDeformerNode* poDeformerNode, int32_t boneIndex)
	{
		// Does nothing here, as it has no mesh?
	}

	DirectX::BoundingBox& DxBaseNode::NodeBoundingBox()
	{
		return m_BoundBox;
	}

	// TODO: used? Remove?
	void DxBaseNode::SetBoundingBox(DxResourceManager& resourcemanager, DirectX::XMFLOAT3 minPoint, DirectX::XMFLOAT3 maxPoint, float stuff)
	{
		DirectX::XMVECTOR xmMin = DirectX::XMLoadFloat3(&minPoint);
		DirectX::XMVECTOR xmMax = DirectX::XMLoadFloat3(&maxPoint);

		DirectX::BoundingBox::CreateFromPoints(NodeBoundingBox(), xmMin, xmMax);

		auto newSimpleShaderProgram =
			rldx::DxMeshShaderProgram::CreateFromDisk<rldx::DxMeshShaderProgram>(
				rldx::DxDeviceManager::Device(),
				resourcemanager,
				LR"(VS_Simple.cso)",
				LR"(PS_Simple.cso)"
			);

		auto bbdMeshData = rldx::DxMeshCreatorHelper::MakeBoundingBoxMesh(rldx::DxDeviceManager::Device(), NodeBoundingBox(), boundBoxRenderColor);

		auto newMeshHandle = resourcemanager.CreateResouce<DxMesh>();
		m_boundingBoxMesh.poMesh = newMeshHandle;
		m_boundingBoxMesh.poMesh->SetMeshData(bbdMeshData);
		m_boundingBoxMesh.poShaderProgram = DefaultShaderCreator::GetSimpleShaderProgram(resourcemanager);
		m_boundingBoxMesh.meshName = L"BoundBox";
	}

	void DxBaseNode::SetDeformerNodeRecursive(const DxDeformerNode* poDeformerNode, int32_t boneIndex)
	{
		SetDeformerNode(poDeformerNode, boneIndex);

		for (auto& itChild : GetChildren())
		{
			itChild->SetDeformerNodeRecursive(poDeformerNode, boneIndex);
		}
	}

	DxBaseNode::UniquePtr DxBaseNode::Create(const std::wstring& name)
	{
		return std::move(std::make_unique<DxBaseNode>(name));
	}

	// TODOD: should m_nodeName be here, when it derived from TIdentifiable that also has m_nodeName?

	const DxBaseNode* DxBaseNode::GetParent() const
	{
		return m_wpoParent;
	}

	DxBaseNode* DxBaseNode::GetParent()
	{
		return m_wpoParent;
	}

	size_t DxBaseNode::GetChildCount() const
	{
		return m_children.size();
	}

	void DxBaseNode::RemoveThis()
	{
		auto parent = GetParent();
		if (parent)
		{
			parent->RemoveChild(this);
		}
	}

	void DxBaseNode::RemoveNode(DxBaseNode* poChild)
	{
		if (poChild == nullptr) return;

		auto parent = GetParent();
		if (!parent) return;

		parent->RemoveChild(this);
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
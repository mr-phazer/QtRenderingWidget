#pragma once

// TODO: clean? are both math libs neeeded
#include <DirectXMath.h>
#include <SimpleMath.h>

#include <Rldx\Managers\DxDeviceManager.h>
#include "..\..\DataTypes\DxMeshData.h"
#include "..\..\Rendering\DxMaterial.h"
#include "..\..\Rendering\DxMeshRenderData.h"
#include "DxBaseNode.h"

namespace rldx {

	// forward decl	
	struct DxMeshData;
	class DxMeshShaderProg;
	class DxSourceDeformerNode;

	class DxMeshNode : public DxBaseNode
	{
	protected:
		DxMeshData m_meshData;

	public:
		using SharedPtr = std::shared_ptr<DxMeshNode>;
	public:
		DxMeshNode() = default;
		DxMeshNode(const std::wstring& name) : DxBaseNode(name) {}

		// TODO: maybe add d3d device as param here also?, the "global" d3d devices might make the code "spaghetti"
		static SharedPtr Create(const std::wstring& name = L"");

		DxMeshData& MeshData() { return m_meshData; }
		void Clone(DxMeshData& clone) const;


		void SetMeshData(const rldx::DxCommonMeshData& meshData, std::wstring meshName, sm::Matrix mWeaponMatrix = sm::Matrix::Identity);
		void SetMeshPivot(const sm::Vector3& pivot) { m_meshData.pivot = pivot; }
		void SetMeshWorldTransForm(const sm::Matrix& mWorld) { m_meshData.perMesh_VS_CB.data.mWeaponWorld = mWorld; }
		void SetMeshVisbilityDistance(float distance) { m_meshData.visibilityDistance = distance; }
		virtual void SetNodeWorldTransForm(const sm::Matrix& mWorld) { m_meshData.perMesh_VS_CB.data.mNodeWorld = mWorld; }

		virtual void SetDeformerNode(const rldx::DxDeformerNode* poDeformerNode, int32_t boneIndex) override;


		virtual void SetAttachBoneAsParent();
		virtual void SetShaderProgram(DxMeshShaderProgram* shaderProgram);

		void SetMaterial(rldx::DxMaterial* pDxMaterial) { m_meshData.poMaterial = pDxMaterial; };
		void SetBoundingBox(DirectX::XMFLOAT3 minPoint, DirectX::XMFLOAT3 maxPoint);
		void SetBoundingBox(const DirectX::BoundingBox& inBB);

		void FlushToRenderBucket(IRenderBucket* pRenderQueue) override;

		virtual SceneNodeTypeEnum GetType() const override { return SceneNodeTypeEnum::MeshNode; }
		virtual std::wstring GetTypeString() const override { return L"DxMeshNode"; };

		virtual void Update(float time) override;
	};
};
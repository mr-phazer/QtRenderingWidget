#pragma once

#include <DirectXMath.h>

#include "..\..\DataTypes\DxMeshData.h"
#include "..\..\Rendering\DxMaterial.h"
#include "..\..\Rendering\DxMeshRenderData.h"
#include "DxBaseNode.h"

namespace rldx {

	// forward decl	
	struct DxMeshRenderData;
	class DxMeshShaderProgram;


	class DxMeshNode : public DxBaseNode
	{

		DxMeshRenderData m_meshData;

	public:
		using SharedPtr = std::shared_ptr<DxMeshNode>;
	public:
		static SharedPtr Create(const std::wstring& name = L"");

		void SetModelData(const rldx::DxCommonMeshData& meshData);
		virtual void SetShaderProgram(DxMeshShaderProgram* shaderProgram) { m_meshData.m_poShaderProgram = shaderProgram; };
		void SetMaterial(rldx::DxMaterial* pDxMaterial) { m_meshData.m_poMaterial = pDxMaterial; };
		void SetBoundingBox(DirectX::XMFLOAT3 minPoint, DirectX::XMFLOAT3 maxPoint);

		void FlushToRenderBucket(IRenderBucket* pRenderQueue) override;

		virtual SceneNodeTypeEnum GetType() const override { return SceneNodeTypeEnum::MeshNode; }
		virtual std::wstring GetTypeString() const override { return L"DxMeshNode"; };

	};
};


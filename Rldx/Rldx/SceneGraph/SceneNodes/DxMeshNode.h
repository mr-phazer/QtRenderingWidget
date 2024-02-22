#pragma once

#include <DirectXMath.h>

#include "..\..\Rendering\DxMeshRenderData.h"
#include "..\..\Rendering\DxMaterial.h"
#include "DxBaseNode.h"
#include "..\..\DataTypes\DxMeshData.h"

namespace rldx {		

	// forward decl	
	class DxMeshRenderData;	
	class DxMeshShaderProgram;	
	

	class DxMeshNode : public DxBaseNode
	{			
	
		DxMeshRenderData m_meshData;

	public:
		using SharedPtr = std::shared_ptr<DxMeshNode>;
	public:
		static SharedPtr Create(const std::string& name = "");		

		void SetModelData(const rldx::DxCommonMeshData& meshData);	
		virtual void SetShaderProgram(DxMeshShaderProgram* shaderProgram) { m_meshData.m_poShaderProgram = shaderProgram; };
		void SetMaterial(rldx::DxMaterial* pDxMaterial) { m_meshData.m_poMaterial = pDxMaterial; };
		void SetBoundingBox(DirectX::XMFLOAT3 minPoint, DirectX::XMFLOAT3 maxPoint);		

		void FlushToRenderBucket(IRenderBucket* pRenderQueue) override;

		SceneNodeTypeEnum GetType() const override { return SceneNodeTypeEnum::MeshNode; }
		std::string GetTypeString() const override { return "DxMeshNode"; };	
		
	};
};


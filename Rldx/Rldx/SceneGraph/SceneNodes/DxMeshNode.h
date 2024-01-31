#pragma once

#include "..\..\Rendering\DxMeshDrawable.h"
#include "..\..\Rendering\DxMaterial.h"
#include "DxBaseNode.h"
#include "..\..\DataTypes\DxMeshData.h"

namespace rldx {		

	// forward decl	
	class DxMeshData;	
	class DxMeshShaderProgram;	

	class DxMeshNode : public DxBaseNode
	{		
		DxMeshDrawable m_meshDrawable;

	public:
		using SharedPointer = std::shared_ptr<DxMeshNode>;
	public:
		static SharedPointer Create(const std::string& name = "");		

		void SetMeshData(const rldx::DxCommonMeshData& meshData);
		void SetShaderProgram(DxMeshShaderProgram* shaderProgram) { m_meshDrawable.m_poShaderProgram = shaderProgram; };
		void SetMaterial(rldx::DxMaterial* pDxMaterial) { m_meshDrawable.m_poMaterial = pDxMaterial; };
		
		void FlushToRenderQueue(IRenderBucket* pRenderQueue) override;

		SceneNodeTypeEnum GetType() const override { return SceneNodeTypeEnum::MeshNode; }
		std::string GetTypeString() const override { return "DxMeshNode"; };	
		
	};
};


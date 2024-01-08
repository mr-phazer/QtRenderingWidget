#pragma once

#include "..\..\Rendering\DxMeshDrawable.h"
#include "..\BaseNode\DxBaseNode.h"
#include "K:\Coding\repos\QtRenderingWidget_RPFM\QtRenderingWidget\Rldx\DataTypes\DxMeshData.h"

namespace Rldx {		

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
		
		void SetShaderProgram(DxMeshShaderProgram* shaderProgram) { m_meshDrawable.m_poShaderProgram = shaderProgram; };

		void SetMeshData(const Rldx::DxCommonMeshData& meshData);
		
		// implemation of type/string
		SceneNodeTypeEnum GetType() const override { return SceneNodeTypeEnum::MeshNode; }
		std::string GetTypeString() const override { return "DxMeshNode"; };
				
		// udpate/flush impl
		void Update(float timeElapsed) override;
		void FlushToRenderQueue(IRenderBucket* pRenderQueue) override;		
	};
};


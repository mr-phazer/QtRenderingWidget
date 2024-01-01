#pragma once

#include "DxSceneNode.h"
#include "..\..\Rendering\DxMesh.h"
#include "..\..\..\Logging\Logging.h"
#include "..\..\SceneGraph\BaseNode\DxBaseNode.h"

#include "..\..\Interfaces\IDrawable.h"
#include "..\..\Interfaces\IBindable.h"
#include "..\..\Interfaces\IUpdateable.h"

#include "..\..\Rendering\DxConstBuffer.h"
#include "..\..\DataTypes\DerformerData.h"

namespace Rldx {

	typedef TDxVSConstBuffer<DxDeformerData> DxDeformerDataBuffer;
		
	class DxMeshNode : public DxBaseNode
	{		
		DxMesh* m_meshData = nullptr;
		DxMaterial* m_material = nullptr;
		DxMeshShaderProgram* m_shaderProgram = nullptr;
		DxDeformerDataBuffer m_deformerDataBuffer;

		// DxBoundingBox
		// DxNodeCube

		DxMeshNode* m_pDeformerNode = nullptr; // points to skeleton node, on rendering, get pointer to actual derformer 


	public:
		using SharedPointerMesh = std::shared_ptr<DxMeshNode>;
	public:
		static SharedPointerMesh Create(const std::string& name = "");
		
		void SetShaderProgram(DxMeshShaderProgram* shaderProgram) { m_shaderProgram = shaderProgram; };

		void SetMeshData(const DxCommonMeshData& meshData)
		{
			auto newMeshHandle = DxResourceManager::Instance()->AllocMesh();
			m_meshData = newMeshHandle.GetPtr();
			m_meshData->SetMeshData(meshData);

			// TODO: REMOVE:
			auto DEBUG_BREAK = 1;
		};		
		
		// implemation of type/string
		SceneNodeTypeEnum GetType() const override { return SceneNodeTypeEnum::MeshNode; }
		std::string GetTypeString() const override { return "DxMeshNode"; };
				
		// udpate/flush impl
		void Update(float timeElapsed) override;
		void FlushToRenderQueue(IRenderBucket* pRenderQueue) override;		
	};
};


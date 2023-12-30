#pragma once

#include "DxSceneNode.h"
#include "..\..\DxObjects\DxMeshData.h"
#include "..\..\..\Logging\Logging.h"
#include "..\..\SceneGraph\BaseNode\DxBaseNode.h"

namespace Rldx 
{

	class DxMeshNode : public DxBaseNode
	{		
		DxMesh* m_meshData = nullptr;
		DxMaterial* m_material = nullptr;
		DxMeshShaderProgram* m_shaderProgram = nullptr;
		// DxBoundingBox
		// DxNodeCube

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
		void FlushToRenderQueue(IDxRenderQueue* pRenderQueue) override;		
	};
};


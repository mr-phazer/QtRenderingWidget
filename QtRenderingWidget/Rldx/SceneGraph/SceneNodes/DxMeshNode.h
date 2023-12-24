#pragma once

#include "DxSceneNode.h"
#include "..\..\DxObjects\DxMesh.h"
#include "..\..\..\Logging\Logging.h"

namespace Rldx
{
	class DxMeshNode final : public IDxSceneNode, IDrawable 
	{
		DxMesh* m_meshData = nullptr;
		DxMaterial* m_material = nullptr;
		DxMeshShaderProgram* m_shaderProgram = nullptr;

	public:
		using Sptr = std::shared_ptr<DxMeshNode>;
	public:
		static DxMeshNode::Sptr Create(const std::string& name = "");

		void SetShaderProgram(DxMeshShaderProgram* shaderProgram) { m_shaderProgram = shaderProgram; };

		void SetMeshData(const DxCommonMeshData& meshData)
		{
			auto newMeshHandle = DxResourceManager::Instance()->AllocMesh();
			m_meshData = newMeshHandle.GetPtr();
			m_meshData->SetMeshData(meshData);

			// TODO: REMOVE:
			auto DEBUG_BREAK = 1;
		};

		void Draw(ID3D11DeviceContext* poDC) override;;
		
		SceneNodeTypeEnum GetType() const override { return SceneNodeTypeEnum::Mesh; }
		std::string GetTypeString() const override { return "DxMeshNode"; };
	};
};
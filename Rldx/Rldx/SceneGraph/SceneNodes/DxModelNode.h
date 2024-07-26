#pragma once

#include <ImportExport\FileFormats\RigidModel\Types\Common\RigidModelFile.h>
#include <ImportExport\FileFormats\WsModel\DataTypes\WsModelData.h>
#include "DxMeshNode.h"

namespace rldx
{
	class IRenderBucket;

	/// <summary>
	/// Holds multiple meshes to make up a complete model.	
	/// </summary>
	class DxModelNode : public DxMeshNode
	{
	public:
		size_t m_activeLod = 0;;
		std::vector<std::vector<DxMeshNode::UniquePtr>> m_lods;
	public:
		using UniquePtr = std::unique_ptr<DxModelNode>;

	public:
		virtual ~DxModelNode()
		{
			WriteDebugDestructMsg();
		}

		DxModelNode() : DxMeshNode(L"Unnamed DxModelNode")
		{
			SetType(SceneNodeTypeEnum::ModelNode);
			SetTypeString(L"Node:DxModelNode");
		}

		//virtual ~DxModelNode()
		//{
		//	WriteDebugDestructMsg();
		//	m_lods.clear();
		//}



		DxModelNode(const std::wstring& m_nodeName) : DxMeshNode(m_nodeName) {}

		void SetModelData(ID3D11Device* poDevice, rldx::DxResourceManager& resourceManager, const rmv2::RigidModelFileCommon& rmv2File);



		virtual void SetShaderProgram(DxMeshShaderProgram* shaderProgram) override;
		virtual void SetDeformerNode(const rldx::DxDeformerNode* poDeformerNode, int32_t boneIndex) override;

		/// <summary>
		/// Set the deformer for this node and all its children recursively
		/// </summary>
		/// <param m_nodeName="poDeformerSourceNode"></param>


		virtual void SetAttachBone(int32_t boneIndex) override;
		virtual void SetAttachBoneAsParent() override;
		// TODO add support for (parsed) WSMODEL
		void LoadMaterialDataFromRmv2(ID3D11Device* poDevice, rldx::DxResourceManager& resourceManager, const rmv2::RigidModelFileCommon& rmv2File);;

		// TODO: TEEEEESST
		void LoadMaterialFromWSmodel(ID3D11Device* poDevice, rldx::DxResourceManager& resourceManager, rmv2::WsModelData& wsModelData);

		virtual void FlushToRenderBucket(IRenderBucket* pRenderQueue) override;
		void FlushModelMeshesToRenderBucked(IRenderBucket* pRenderQueue);

		virtual void SetNodeWorldTransForm(const sm::Matrix& mWorld);

		virtual void SetDrawState(DrawStateEnum state) override
		{
			DxBaseNode::SetDrawState(state);

			for (auto& lod : m_lods)
			{
				for (auto& meshNode : lod)
				{
					meshNode->SetDrawState(state);
				};
			};
		}

	private:
		// TODO: rename to "load"/"allocate"/"createDxBuffersfrom", as "set" sounds like it just a simple copying
		void SetSingleMesh(
			ID3D11Device* poDevice,
			rldx::DxResourceManager& resourceManager,
			DxMeshNode::UniquePtr& upoMesh,
			const rmv2::LODHeaderCommon& lodHeader,
			const rmv2::MeshHeaderType3& meshHeader,
			const rmv2::MaterialHeaderType5& materialHeader,
			const rmv2::MeshBlockCommon& rmr2MeshData);

		/// <summary>
		/// Resizes the bound box to fit all the meshes in the container
		/// </summary>
		virtual void ResizeBoundBoxToContent() override;
	};
};
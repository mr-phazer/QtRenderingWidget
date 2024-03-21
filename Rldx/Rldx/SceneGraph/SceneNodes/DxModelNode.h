#pragma once

#include "..\..\ImportExport\FileFormats\RigidModel\Types\Common\RigidModelFile.h"
#include "..\..\ImportExport\FileFormats\WsModel\DataTypes\WsModelData.h"
#include "DxMeshNode.h"

namespace rldx
{
	class IRenderBucket;

	/// <summary>
	/// Holds multiple meshes to make up a complete model.	
	/// </summary>
	class DxModelNode : public DxMeshNode
	{
	protected:
		size_t m_activeLod = 0;;
		std::vector<std::vector<DxMeshNode::SharedPtr>> m_lods;
	public:
		using SharedPtr = std::shared_ptr<DxModelNode>;

	public:
		void SetModelData(ID3D11Device* poDevice, const rmv2::RigidModelFileCommon& rmv2File);;

		virtual void SetShaderProgram(DxMeshShaderProgram* shaderProgram) override;
		virtual void SetDeformerNode(const rldx::DxDeformerNode* poDeformerNode, int32_t boneIndex) override;

		/// <summary>
		/// Set the deformer for this node and all its children recursively
		/// </summary>
		/// <param name="poDeformerNode"></param>


		virtual void SetAttachBone(int32_t boneIndex) override;
		virtual void SetAttachBoneAsParent() override;
		// TODO add support for (parsed) WSMODEL
		void LoadMaterialDataFromRmv2(ID3D11Device* poDevice, const rmv2::RigidModelFileCommon& rmv2File);;

		// TODO: TEEEEESST
		void LoadMaterialFromWSmodel(ID3D11Device* poDevice, rmv2::WsModelData& wsModelData);

		virtual void FlushToRenderBucket(IRenderBucket* pRenderQueue) override;;

	private:
		void SetSingleMesh(
			ID3D11Device* poDevice,
			size_t iLod,
			size_t iMesh,
			const rmv2::MeshHeaderType3& meshHeader,
			const rmv2::MaterialHeaderType5& materialHeader,
			const rmv2::MeshBlockCommon& rmr2MeshData);
	};
};
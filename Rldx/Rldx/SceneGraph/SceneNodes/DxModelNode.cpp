#include "DxModelNode.h"

#include "..\..\Helpers\DxMeshCreatorHelper.h"

using namespace utils;

namespace rldx {

	void DxModelNode::SetShaderProgram(DxMeshShaderProgram* shaderProgram)
	{
		for (auto& lod : m_lods)
		{
			for (auto& meshNode : lod)
			{
				meshNode->SetShaderProgram(shaderProgram);
			};
		};
	}

	void DxModelNode::SetModelData(ID3D11Device* poDevice, rldx::DxResourceManager& resourceManager, const rmv2::RigidModelFileCommon& rmv2File)
	{
		// fills "model" with indivual meshes
		m_lods.resize(rmv2File.fileHeader.wLodCount);
		for (size_t iLod = 0; iLod < rmv2File.fileHeader.wLodCount; iLod++)
		{
			m_lods[iLod].resize(rmv2File.lodHeaders[iLod].dwMeshCount);
			for (size_t iMesh = 0; iMesh < m_lods[iLod].size(); iMesh++)
			{
				SetSingleMesh(
					poDevice,
					resourceManager,
					m_lods[iLod][iMesh],
					rmv2File.lodHeaders[iLod],
					rmv2File.lods[iLod].meshBlocks[iMesh].meshHeader,
					rmv2File.lods[iLod].meshBlocks[iMesh].materialBlock.materialHeader,
					rmv2File.lods[iLod].meshBlocks[iMesh]);
			}
		}

		DxModelNode::ResizeBoundBoxToContent();

		UpdateParentBoundBoxesTraverseUp();
	}

	void DxModelNode::ResizeBoundBoxToContent()
	{
		for (auto& itLod : m_lods)
			for (auto& itMesh : itLod)
			{
				DirectX::BoundingBox::CreateMerged(
					NodeBoundingBox(),
					NodeBoundingBox(),
					itMesh->NodeBoundingBox());
			}
	}

	void DxModelNode::SetDeformerNode(const rldx::DxDeformerNode* poDeformerNode, int32_t boneIndex)
	{
		DxMeshNode::SetDeformerNode(poDeformerNode, boneIndex); // sets the derformer using base class

		for (auto& itLod : m_lods)
		{
			for (auto& itMeshNode : itLod)
			{
				itMeshNode->SetDeformerNode(poDeformerNode, boneIndex); // recursive
			};
		};
	}

	void DxModelNode::SetAttachBone(int32_t boneIndex)
	{
		DxMeshNode::SetAttachBone(boneIndex);

		for (auto& itLod : m_lods)
		{
			for (auto& itMeshNode : itLod)
			{
				itMeshNode->SetAttachBone(boneIndex);
			};
		};
	}

	void DxModelNode::SetAttachBoneAsParent()
	{
		DxMeshNode::SetAttachBoneAsParent();

		for (auto& itLod : m_lods)
		{
			for (auto& itMeshNode : itLod)
			{
				DxMeshNode::SetAttachBoneAsParent();
			};
		};
	}

	void DxModelNode::SetSingleMesh(ID3D11Device* poDevice, rldx::DxResourceManager& resourceManager, DxMeshNode::UniquePtr& upoMeshNode, const rmv2::LODHeaderCommon& lodHeader, const rmv2::MeshHeaderType3& meshHeader, const rmv2::MaterialHeaderType5& materialHeader, const rmv2::MeshBlockCommon& rmr2MeshData)
	{
		// TODO: maybe nodes to tree add to tree, for easier bound-box -> boundvolumne calculation
		// TODO: or simply calculate the bounding volumen for parent
		upoMeshNode = std::make_unique<DxMeshNode>(ToWString(materialHeader.szMeshName));

		auto rm2MeshData = DxMeshCreatorHelper::CreateFromRmv2Mesh(poDevice, rmr2MeshData);

		upoMeshNode->SetMeshData(resourceManager, rm2MeshData, ToWString(materialHeader.szMeshName));
		upoMeshNode->SetMeshPivot(materialHeader.transforms.vPivot);
		upoMeshNode->SetMeshVisbilityDistance(lodHeader.fVisibilityDistance);
		upoMeshNode->ResizeBoundBoxToContent();
		DxBaseNode::AllocateBoundingBoxMesh(upoMeshNode.get(), resourceManager);

	}

	void DxModelNode::LoadMaterialDataFromRmv2(ID3D11Device* poDevice, rldx::DxResourceManager& resourceManager, const rmv2::RigidModelFileCommon& rmv2File)
	{
		size_t iLod = 0;
		m_lods.resize(1);
		m_lods[iLod].resize(rmv2File.lodHeaders[0].dwMeshCount);

		for (size_t iMesh = 0; iMesh < m_lods[0].size(); iMesh++)
		{
			DxMaterial* material = MaterialCreatorRMV2Mesh(rmv2File.lods[iLod].meshBlocks[iMesh]).Create(poDevice, resourceManager);
			m_lods[iLod][iMesh]->SetMaterial(material);
		}
	}

	void DxModelNode::LoadMaterialFromWSmodel(ID3D11Device* poDevice, rldx::DxResourceManager& resourceManager, rmv2::WsModelData& wsModelData)
	{
		if (wsModelData.xmlMateriData.empty()) {
			throw std::exception("lod_size == 0, unexpected!");
		}

		size_t iLod = 0;

		if (wsModelData.xmlMateriData[iLod].size() != m_lods[iLod].size()) {
			throw std::exception("mateial.mesh_count != mesh_count, unexpected!");
		}

		for (size_t iMesh = 0; iMesh < wsModelData.xmlMateriData[0].size(); iMesh++)
		{
			auto material = DxMaterial::Create(wsModelData.xmlMateriData[0][iMesh].textures, resourceManager);
			m_lods[0][iMesh]->SetMaterial(material);
		}
	}

	void DxModelNode::FlushToRenderBucket(IRenderBucket* pRenderQueue)
	{
		FlushModelMeshesToRenderBucked(pRenderQueue);
	}

	void DxModelNode::FlushModelMeshesToRenderBucked(IRenderBucket* pRenderQueue)
	{
		if (m_activeLod >= m_lods.size() || GetDrawState() != DrawStateEnum::Draw) {
			return;
		}

		for (auto& itMeshNode : m_lods[m_activeLod])
		{
			itMeshNode->DxMeshNode::FlushToRenderBucket(pRenderQueue);
		};
	}

	void DxModelNode::SetNodeWorldTransForm(const sm::Matrix& mWorld)
	{
		DxMeshNode::SetNodeWorldTransForm(mWorld);

		for (auto& lod : m_lods)
		{
			for (auto& meshNode : lod)
			{
				meshNode->SetNodeWorldTransForm(mWorld);
			};
		};
	}
}
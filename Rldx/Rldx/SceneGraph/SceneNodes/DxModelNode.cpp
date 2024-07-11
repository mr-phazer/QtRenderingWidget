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

	void DxModelNode::SetModelData(ID3D11Device* poDevice, const rmv2::RigidModelFileCommon& rmv2File)
	{
		m_lods.resize(rmv2File.fileHeader.wLodCount);
		for (size_t iLod = 0; iLod < rmv2File.fileHeader.wLodCount; iLod++)
		{
			m_lods[iLod].resize(rmv2File.lodHeaders[iLod].dwMeshCount);
			for (size_t iMesh = 0; iMesh < m_lods[iLod].size(); iMesh++)
			{
				SetSingleMesh(
					poDevice,
					iLod,
					iMesh,
					rmv2File.lodHeaders[iLod],
					rmv2File.lods[iLod].meshBlocks[iMesh].meshHeader,
					rmv2File.lods[iLod].meshBlocks[iMesh].materialBlock.materialHeader,
					rmv2File.lods[iLod].meshBlocks[iMesh]
				);
			}
		}

		UpdateAllBoundBoxes();
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

	void DxModelNode::SetSingleMesh(ID3D11Device* poDevice, size_t iLod, size_t iMesh, const rmv2::LODHeaderCommon& lodHeader, const rmv2::MeshHeaderType3& meshHeader, const rmv2::MaterialHeaderType5& materialHeader, const rmv2::MeshBlockCommon& rmr2MeshData)
	{
		m_lods[iLod][iMesh] = DxMeshNode::Create(ToWString(materialHeader.szMeshName));
		auto rm2MeshData = DxMeshCreatorHelper::CreateFromRmv2Mesh(poDevice, rmr2MeshData);
		m_lods[iLod][iMesh]->SetMeshData(rm2MeshData, ToWString(materialHeader.szMeshName));
		m_lods[iLod][iMesh]->SetMeshPivot(materialHeader.transforms.vPivot);
		m_lods[iLod][iMesh]->SetMeshVisbilityDistance(lodHeader.fVisibilityDistance);

		// Iinit the bound from RigidModelV2 header
		m_lods[iLod][iMesh]->SetBoundingBox(
			meshHeader.vMinBB,
			meshHeader.vMaxBB
		);

		FitChildBoundBox(m_lods[iLod][iMesh].get());
	}

	void DxModelNode::LoadMaterialDataFromRmv2(ID3D11Device* poDevice, const rmv2::RigidModelFileCommon& rmv2File)
	{
		size_t iLod = 0;
		m_lods.resize(1);
		m_lods[iLod].resize(rmv2File.lodHeaders[0].dwMeshCount);

		for (size_t iMesh = 0; iMesh < m_lods[0].size(); iMesh++)
		{
			DxMaterial* material = MaterialCreatorRMV2Mesh(rmv2File.lods[iLod].meshBlocks[iMesh]).Create(poDevice);
			m_lods[iLod][iMesh]->SetMaterial(material);
		}
	}

	void DxModelNode::LoadMaterialFromWSmodel(ID3D11Device* poDevice, rmv2::WsModelData& wsModelData)
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
			auto material = DxMaterial::Create(wsModelData.xmlMateriData[0][iMesh].textures);
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
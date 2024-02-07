#pragma once

#include "DxMeshNode.h"
#include "..\..\Interfaces\IRenderBucket.h"
#include "..\..\ImportExport\RigidModel\DataTypes\RigidModelFile.h"
#include "..\..\Creators\DxMeshCreator.h"

namespace rldx
{
	/// <summary>
	/// Holds multiple meshes make up a complete model.
	/// </summary>
	class DxModelNode : public DxMeshNode
	{
		size_t m_activeLod = 0;;
		std::vector<std::vector<DxMeshNode::SharedPtr>> m_lods =
		{
			{DxMeshNode::SharedPtr()}
		};

	public:
		void SetMeshNodes(size_t lodIndex, std::vector<std::vector<DxMeshNode::SharedPtr>> lods)
		{
			m_lods = lods;
		};

		void SetShaderProgram(DxMeshShaderProgram* shaderProgram)
		{
			for (auto& lod : m_lods)
			{
				for (auto& meshNode : lod)
				{
					meshNode->SetShaderProgram(shaderProgram);
				};
			};
		};

		void SetNewModel(ID3D11Device* poDevice, const rmv2::RigidModelFileCommon& rmv2File)
		{
			// TODO: loading only LOD 0, load all?
			size_t iLod = 0; 

			// TODO: lod all LODs?
			//m_lods.resize(rmv2File.fileHeader.wLodCount);

			m_lods.resize(1);

			// TODO: this for loading all LODs
			//for (size_t iLod = 0; iLod < m_lods.size(); iLod++)

			m_lods[iLod].resize(rmv2File.lodHeaders[iLod].dwMeshCount);			
			for (size_t iMesh = 0; iMesh < m_lods[iLod].size(); iMesh++)
			{
				SetSingleMesh(
					poDevice,
					iLod,
					iMesh,
					rmv2File.lods[iLod].meshBlocks[iMesh].meshHeader,
					rmv2File.lods[iLod].meshBlocks[iMesh].materialBlock.materialHeader,
					rmv2File.lods[iLod].meshBlocks[iMesh]
				);
			}		
		};

		void SetSingleMesh(
			ID3D11Device* poDevice, 
			size_t iLod, 
			size_t iMesh, 
			const rmv2::MeshHeaderType3& meshHeader, 
			const rmv2::MaterialHeaderType5& materialHeader, 
			const rmv2::MeshBlockCommon& rmr2MeshData)
		{

			m_lods[iLod][iMesh] = rldx::DxMeshNode::Create(materialHeader.szMeshName);
			auto rm2MeshData = rldx::DxMeshCreatorHelper::CreateFromRmv2Mesh(poDevice, rmr2MeshData);
			m_lods[iLod][iMesh]->SetNewModel(rm2MeshData);

			// Iinit the bound from RMV2 header
			m_lods[iLod][iMesh]->SetBoundingBox(
				meshHeader.vMinBB,
				meshHeader.vMaxBB
			);

			// Model's BoundBox has to contain all the mesh bound boxes:
			DirectX::BoundingBox::CreateMerged(
				this->GetBoundingBox(),
				this->GetBoundingBox(),
				m_lods[iLod][iMesh]->GetBoundingBox());
		};

		// TODO add support for (parsed) WSMODEL
		void LoadMaterialData(ID3D11Device* poDevice, const rmv2::RigidModelFileCommon& rmv2File)
		{
			size_t iLod = 0;
			m_lods.resize(1);
			m_lods[iLod].resize(rmv2File.lodHeaders[0].dwMeshCount);

			for (size_t iMesh = 0; iMesh < m_lods[0].size(); iMesh++)
			{
				DxMaterial* material = MaterialCreatorRMV2Mesh(rmv2File.lods[iLod].meshBlocks[iMesh]).Create(poDevice);
				m_lods[iLod][iMesh]->SetMaterial(material);

			}
		};

	public:
		virtual void FlushToRenderQueue(IRenderBucket* pRenderQueue) override
		{
			for (auto& itMeshNode : m_lods[m_activeLod])
			{
				itMeshNode->FlushToRenderQueue(pRenderQueue);
			};
		};
	};
};
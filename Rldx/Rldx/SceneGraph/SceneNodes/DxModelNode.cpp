#include "DxModelNode.h"

#include "..\..\Creators\DxMeshCreator.h"

void rldx::DxModelNode::SetShaderProgram(DxMeshShaderProgram* shaderProgram)
{
	for (auto& lod : m_lods)
	{
		for (auto& meshNode : lod)
		{
			meshNode->SetShaderProgram(shaderProgram);
		};
	};
}

void rldx::DxModelNode::SetModelData(ID3D11Device* poDevice, const rmv2::RigidModelFileCommon& rmv2File)
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
}

void rldx::DxModelNode::SetSingleMesh(ID3D11Device* poDevice, size_t iLod, size_t iMesh, const rmv2::MeshHeaderType3& meshHeader, const rmv2::MaterialHeaderType5& materialHeader, const rmv2::MeshBlockCommon& rmr2MeshData)
{

	m_lods[iLod][iMesh] = rldx::DxMeshNode::Create(materialHeader.szMeshName);
	auto rm2MeshData = rldx::DxMeshCreatorHelper::CreateFromRmv2Mesh(poDevice, rmr2MeshData);
	m_lods[iLod][iMesh]->SetModelData(rm2MeshData);

	// Iinit the bound from RigidModelV2 header
	m_lods[iLod][iMesh]->SetBoundingBox(
		meshHeader.vMinBB,
		meshHeader.vMaxBB
	);

	// Model's BoundBox has to contain all the mesh bound boxes:
	DirectX::BoundingBox::CreateMerged(
		this->GetNodeBoundingBox(),
		this->GetNodeBoundingBox(),
		m_lods[iLod][iMesh]->GetNodeBoundingBox());
}


void rldx::DxModelNode::LoadMaterialDataFromRmv2(ID3D11Device* poDevice, const rmv2::RigidModelFileCommon& rmv2File)
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


void rldx::DxModelNode::LoadMaterialFromWSmodel(ID3D11Device* poDevice, rmv2::WsModelData& wsModelData)
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

void rldx::DxModelNode::FlushToRenderBucket(IRenderBucket* pRenderQueue)
{
	if (m_activeLod >= m_lods.size() || GetDrawState() != DrawStateEnum::Draw) {
		return;
	}

	for (auto& itMeshNode : m_lods[m_activeLod])
	{
		itMeshNode->FlushToRenderBucket(pRenderQueue);
	};
}

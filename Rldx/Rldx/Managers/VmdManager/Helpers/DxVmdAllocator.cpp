#include "DxVmdAllocator.h"

#include "..\..\..\Creators\DxGameShaderCreators.h"
#include "..\..\DxDeviceManager.h"
#include "..\..\ImportExport\FileFormats\RigidModel\Readers\RigidModelReader.h"

// TODO: remove?
//void rldx::DxVmdNodeAllocator::AllocateDxBuffers(DxVmdNode* sceneVmdNode)
//{
//	auto& data = sceneVmdNode->VMDTagData();
//
//	switch (data.tagType)
//	{
//	case VMDTagEnum::VariantMesh:
//	{
//		AllocateVariantMesh(sceneVmdNode);
//	}
//	}
//}
//
//void rldx::DxVmdNodeAllocator::AllocateVariantMesh(DxVmdNode* sceneVmdNode)
//{
//	auto& data = sceneVmdNode->VMDTagData();
//
//	auto rmv2Bytes = DxResourceManager::GetCallBackFile(data.varintMeshData.modelPath);
//	auto parsedRmv2File = rmv2::RigidModelReader().Read(rmv2Bytes);
//
//	sceneVmdNode->SetModelData(rldx::DxDeviceManager::Device(), parsedRmv2File);
//
//	if (data.varintMeshData.wsModelData.IsValid())	{ // if there isn't a valid wsmodel for the node, load from the RMV2
//		sceneVmdNode->LoadMaterialFromWSmodel(rldx::DxDeviceManager::Device(), data.varintMeshData.wsModelData);
//	}
//	else { 
//		sceneVmdNode->LoadMaterialDataFromRmv2(rldx::DxDeviceManager::Device(), parsedRmv2File);
//	}
//}

// TODO: remove?
void rldx::DxVmdNodeAllocator::AllocateDxBuffers()
{
	auto& data = m_sceneVmdNode->VMDTagData();

	m_sceneVmdNode->SetDrawState(DxBaseNode::DrawStateEnum::Draw);

	switch (data.tagType)
	{
	case VMDTagEnum::VariantMesh:
	{
		AllocateVariantMesh();
	}
	}
}

void rldx::DxVmdNodeAllocator::AllocateVariantMesh()
{
	auto gameShaderProgramCreator = GameShaderProgramCreatorFactory().Get(DxResourceManager::Instance()->GetGameIdSting());
	if (!gameShaderProgramCreator) {
		throw std::exception("Error loadeinfGame Shader");
	}

	auto newPbrShaderProgram = gameShaderProgramCreator->Create(DxDeviceManager::Device());
	if (!newPbrShaderProgram) {
		throw std::exception("Error Creating Shader");
	}


	auto& data = m_sceneVmdNode->VMDTagData();

	if (data.varintMeshData.wsModelData.geometryPath.empty()) { // a "nesting <VariantMesh>, one per file?
		return;
	}

	auto rmv2Bytes = DxResourceManager::GetCallBackFile(data.varintMeshData.wsModelData.geometryPath);
	auto parsedRmv2File = rmv2::RigidModelReader().Read(rmv2Bytes);

	m_sceneVmdNode->SetModelData(rldx::DxDeviceManager::Device(), parsedRmv2File);

	m_sceneVmdNode->LoadMaterialFromWSmodel(rldx::DxDeviceManager::Device(), data.varintMeshData.wsModelData);
	m_sceneVmdNode->SetShaderProgram(m_nodeShaderProgram);
}


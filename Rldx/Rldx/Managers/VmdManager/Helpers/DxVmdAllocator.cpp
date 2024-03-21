#include "..\..\..\SceneGraph\SceneNodes\DxDeformerNode.h"
#include "DxVmdAllocator.h"

#include "..\..\..\Creators\DxGameShaderCreators.h"
#include "..\..\DxDeviceManager.h"
#include "..\..\ImportExport\FileFormats\RigidModel\Readers\RigidModelReader.h"

// TODO: remove?
//void rldx::DxVmdNodeAllocator::AllocateDxBuffers(DxVmdNode* sceneVmdNode)
//{
//	auto& data = sceneVmdNode->vmdNodeData;
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
//	auto& data = sceneVmdNode->vmdNodeData;
//
//	auto rmv2Bytes = DxResourceManager::GetCallBackFile(data.varintMeshData.modelPath);
//	auto parsedRmv2File = rmv2::RigidModelReader().Read(rmv2Bytes);
//
//	sceneVmdNode->SetMeshData(rldx::DxDeviceManager::Device(), parsedRmv2File);
//
//	if (data.varintMeshData.wsModelData.IsValid())	{ // if there isn't mTransForm valid wsmodel for the node, load from the RMV2
//		sceneVmdNode->LoadMaterialFromWSmodel(rldx::DxDeviceManager::Device(), data.varintMeshData.wsModelData);
//	}
//	else { 
//		sceneVmdNode->LoadMaterialDataFromRmv2(rldx::DxDeviceManager::Device(), parsedRmv2File);
//	}
//}

// TODO: remove?
void rldx::DxVmdNodeAllocator::AllocateDxBuffers(std::string& destSkeletonName, WStringkeyMap<sm::Matrix>& preTransformMap)
{
	auto& data = m_sceneVmdNode->vmdNodeData;

	m_sceneVmdNode->SetDrawState(DxBaseNode::DrawStateEnum::Draw);

	switch (data.tagType)
	{
		case VMDTagEnum::VariantMesh:
		{
			AllocateVariantMesh(destSkeletonName, preTransformMap);
		}

		case VMDTagEnum::Slot:
		{
			// TODO: anything?
				//poDeformer->AttachWeapon(m_sceneVmdNode, m_sceneVmdNode->vmdNodeData.slotData.attcachPointName);
		}
	}
}

void rldx::DxVmdNodeAllocator::AllocateVariantMesh(std::string& destSkeletonName, WStringkeyMap<sm::Matrix>& preTransformMap)
{
	auto gameShaderProgramCreator = GameShaderProgramCreatorFactory().Get(DxResourceManager::Instance()->GetGameIdSting());
	if (!gameShaderProgramCreator) {
		throw std::exception("Error loadeinfGame Shader");
	}

	auto newPbrShaderProgram = gameShaderProgramCreator->Create(DxDeviceManager::Device());
	if (!newPbrShaderProgram) {
		throw std::exception("Error Creating Shader");
	}

	auto& data = m_sceneVmdNode->vmdNodeData;

	if (data.varintMeshData.wsModelData.geometryPath.empty()) { // mTransForm "nesting <VariantMesh>, one per file?
		return;
	}

	auto rmv2Bytes = DxResourceManager::GetCallBackFile(data.varintMeshData.wsModelData.geometryPath);
	auto parsedRmv2File = rmv2::RigidModelReader().Read(rmv2Bytes);

	if (!std::string(parsedRmv2File.fileHeader.szSkeletonId).empty())
	{
		destSkeletonName = parsedRmv2File.fileHeader.szSkeletonId;
	}

	m_sceneVmdNode->SetModelData(rldx::DxDeviceManager::Device(), parsedRmv2File);
	m_sceneVmdNode->LoadMaterialFromWSmodel(rldx::DxDeviceManager::Device(), data.varintMeshData.wsModelData);
	m_sceneVmdNode->SetShaderProgram(m_nodeShaderProgram);

	for (auto& lod : parsedRmv2File.lods)
	{
		for (auto& mesh : lod.meshBlocks)
		{
			for (auto& attach : mesh.materialBlock.attachElements)
			{
				/*sm::Matrix mTransForm = attach.mTransformMatrix3x4;

				preTransformMap[libtools::string_to_wstring(attach.szAttachmentPointName)] = mTransForm;*/
			}
		}
	}

}


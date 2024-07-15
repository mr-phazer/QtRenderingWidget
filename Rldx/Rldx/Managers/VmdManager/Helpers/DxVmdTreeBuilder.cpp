#include "DxMaterialInfoReader.h"
#include "DxVmdTreeBuilder.h"

using namespace rldx;
using namespace rmv2;
using namespace utils;

void SetAttachPointName(DxVmdNode* node)
{
	if (!node) return;
	if (!node->GetParent()) return;

	auto parentNode = dynamic_cast<const DxVmdNode*>(node->GetParent());
	if (!parentNode) return;

	if (node->vmdNodeData.slotData.attcachPointName.empty())
	{
		node->vmdNodeData.slotData.attcachPointName
			= parentNode->vmdNodeData.slotData.attcachPointName; // get the parent's attach point m_nodeName
	}
}

void rldx::DxVmdTreeBuilder::Build(DxVmdNode* sceneGraphNode, const pugi::xml_node& xmlNode)
{
	DxVmdNode* spoNewSceneChild = nullptr;

	if (CompareNoCase(xmlNode.name(), VMDTagStrings::VariantMesh))
	{
		auto newVariantMeshNode = std::make_unique<DxVMDVariantMeshNode>(xmlNode);
		spoNewSceneChild = &sceneGraphNode->AddChild(std::move(newVariantMeshNode));
		DxMaterialInfoReader(&spoNewSceneChild->vmdNodeData).Parse(); // -- Load material info			

	}
	else if (CompareNoCase(xmlNode.name(), VMDTagStrings::Slot))
	{
		auto newSlotNode = std::make_unique<DxVMDSlotNode>(xmlNode);
		spoNewSceneChild = &sceneGraphNode->AddChild(std::move(newSlotNode));
	}
	else if (CompareNoCase(xmlNode.name(), VMDTagStrings::VariantMeshReference))
	{
		LoadVariantMeshRerenceNode(sceneGraphNode, xmlNode);
	}
	else
	{
		// TODO: maybe create a "universal" node type that can handle any type of data, so the VMD can saved from this tree
	}

	SetAttachPointName(spoNewSceneChild);

	for (auto& itXmlChild : xmlNode.children())
	{
		if (spoNewSceneChild) {
			Build(spoNewSceneChild, itXmlChild);
		}
	}
}

void rldx::DxVmdTreeBuilder::LoadVariantMeshRerenceNode(DxVmdNode* sceneGraphNode, const pugi::xml_node& xmlNode)
{
	std::wstring vmdFilePath = xmlNode.attribute_no_case(VMDTagAttributes::Definition).as_string();
	auto vmdBytes = DxResourceManager::GetFile(vmdFilePath);

	if (vmdBytes.GetBufferSize() == 0) {
		sceneGraphNode->vmdNodeData.tagType = VMDTagEnum::INVALID;
		return;
	}

	pugi::xml_document xmlDoc;
	auto xmlParseResult = xmlDoc.load_buffer(vmdBytes.GetBufferPtr(), vmdBytes.GetBufferSize());

	if (!xmlParseResult) {
		throw std::exception(("Create::Read(): XML error: " + std::string(xmlParseResult.description())).c_str());
	}

	sceneGraphNode->SetAttachBoneAsParent();

	DxVmdTreeBuilder().Build(sceneGraphNode, xmlDoc.first_child());
}
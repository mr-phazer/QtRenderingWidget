#include <functional>
#include "DxMaterialInfoReader.h"
#include "DxVmdTreeBuilder.h"

using namespace rldx;
using namespace rmv2;

void rldx::DxVmdTreeBuilder::Build(DxVmdNode* sceneGraphNode, const pugi::xml_node& xmlNode)
{
	m_attachPoints.clear();

	DxVmdNode::SharedPtr spoNewSceneChild = nullptr;

	if (CompareNoCase(xmlNode.name(), VMDTagStrings::VariantMesh))
	{
		spoNewSceneChild = std::make_shared<DxVMDVariantMeshNode>(xmlNode);
		DxMaterialInfoReader(&spoNewSceneChild->VMDTagData()).Parse(); // -- Load material info		
		sceneGraphNode->AddChild(spoNewSceneChild);

	}
	else if (CompareNoCase(xmlNode.name(), VMDTagStrings::Slot))
	{
		spoNewSceneChild = std::make_shared<DxVMDSlotNode>(xmlNode, m_attachPoints); //
		sceneGraphNode->AddChild(spoNewSceneChild);

	}
	else if (CompareNoCase(xmlNode.name(), VMDTagStrings::VariantMeshReference))
	{
		// -- Dont create a child node, make the current node into a "VariantMesh" node, but with the data from the reference file
		LoadVariantMeshRerenceNode(sceneGraphNode, xmlNode);
	}
	else
	{
		// TODO: maybe create a "universal" node type that can handle any type of data, so the VMD can saved from this tree
	}

	for (auto& itXmlChild : xmlNode.children()) {

		if (spoNewSceneChild)
			Build(spoNewSceneChild.get(), itXmlChild);
	}
}

void rldx::DxVmdTreeBuilder::LoadVariantMeshRerenceNode(DxVmdNode* sceneGraphNode, const pugi::xml_node& xmlNode)
{
	std::wstring vmdFilePath = xmlNode.attribute_no_case(VMDTagAttributes::Definition).as_string();
	auto vmdBytes = DxResourceManager::GetCallBackFile(vmdFilePath);

	if (vmdBytes.GetBufferSize() == 0) {
		sceneGraphNode->VMDTagData().tagType = VMDTagEnum::INVALID;
		return;
	}

	pugi::xml_document xmlDoc;
	auto xmlParseResult = xmlDoc.load_buffer(vmdBytes.GetBufferPtr(), vmdBytes.GetBufferSize());

	if (!xmlParseResult) {
		throw std::exception(("Create::Read(): XML error: " + std::string(xmlParseResult.description())).c_str());
	}

	DxVmdTreeBuilder().Build(sceneGraphNode, xmlDoc.first_child());
}
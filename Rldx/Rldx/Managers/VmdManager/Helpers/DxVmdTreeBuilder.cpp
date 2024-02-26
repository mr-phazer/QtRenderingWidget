#include "DxMaterialInfoReader.h"
#include "DxVmdTreeBuilder.h"

using namespace rldx;
using namespace rmv2;

void rldx::DxVmdTreeBuilder::Build(DxVmdNode* sceneGraphNode, const pugi::xml_node& xmlNode)
{
	m_attachPoints.clear();

	if (CompareNoCase(xmlNode.name(), VMDTag::VariantMesh))
	{
		LoadVarintMeshNode(sceneGraphNode, xmlNode);
	}
	else if (CompareNoCase(xmlNode.name(), VMDTag::Slot))
	{
		LoadSlotNode(sceneGraphNode, xmlNode);
	}
	else if (CompareNoCase(xmlNode.name(), VMDTag::VariantMeshReference))
	{
		LoadVariantMeshRerence(sceneGraphNode, xmlNode);
	}
	else
	{
		LoadUnknownNode(sceneGraphNode, xmlNode);
	}

	for (auto& iiVarianMeshsNode : xmlNode.children()) {

		auto newSceneNode = std::make_shared<DxVmdNode>(); // new node for each child node
		sceneGraphNode->AddChild(newSceneNode);
		Build(newSceneNode.get(), iiVarianMeshsNode);
	}
}

void rldx::DxVmdTreeBuilder::LoadUnknownNode(rldx::DxVmdNode* sceneGraphNode, const pugi::xml_node& xmlNode)
{
	sceneGraphNode->GetVMDNodeDataRef().Name = xmlNode.attribute_no_case(VMDTagAtrtibtes::Name).as_string();
}

void rldx::DxVmdTreeBuilder::LoadVarintMeshNode(DxVmdNode* root, const pugi::xml_node& xmlNode)
{
	VMDNodeData& newData = root->GetVMDNodeDataRef();
	newData.Tag = VMDTagEnum::VariantMesh;
	newData.varintMeshData.modelPath = xmlNode.attribute_no_case(VMDTagAtrtibtes::Model).value();
	newData.varintMeshData.imposterModelPath = xmlNode.attribute_no_case(VMDTagAtrtibtes::ImposterModel).value();

	DxMaterialInfoReader(&newData).Parse();
}

void rldx::DxVmdTreeBuilder::LoadSlotNode(DxVmdNode* root, const pugi::xml_node& xmlNode)
{
	VMDNodeData& vmdNodeData = root->GetVMDNodeDataRef();
	vmdNodeData.Tag = VMDTagEnum::Slot;
	vmdNodeData.Name = xmlNode.attribute_no_case(VMDTagAtrtibtes::Name).as_string();
	vmdNodeData.slotData.name = vmdNodeData.Name;
	vmdNodeData.slotData.attcachPointName = xmlNode.attribute_no_case(VMDTagAtrtibtes::AttachPoint).as_string();

	m_attachPoints.push_back(vmdNodeData.slotData.attcachPointName);
}

void rldx::DxVmdTreeBuilder::LoadVariantMeshRerence(DxVmdNode* sceneGraphNode, const pugi::xml_node& xmlNode)
{
	std::wstring vmdFilePath = xmlNode.attribute_no_case(VMDTagAtrtibtes::Definition).as_string();
	auto vmdBytes = DxResourceManager::GetCallBackFile(vmdFilePath);

	auto xmlParseResult = m_xmldoc.load_buffer(vmdBytes.GetBufferPtr(), vmdBytes.GetBufferSize());

	if (!xmlParseResult) {
		throw std::exception(("Create::Read(): XML error: " + std::string(xmlParseResult.description())).c_str());
	}

	LoadVarintMeshNode(sceneGraphNode, m_xmldoc);
}

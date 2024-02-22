#include "VmdNodeCreators.h"

#include "DxMaterialInfoReader.h"
#include "..\..\..\Managers\VmdManager\DxVmdManager.h"

using namespace rldx;
using namespace rmv2;

DxVmdNode::SharedPtr rldx::VariantMeshNodeCreator::Create(DxVmdNode* root, const pugi::xml_node& xmlNode)
{
	//auto newNode = std::make_shared<DxVmdNode>();
	VMDNodeData& newData = root->GetVMDNodeDataRef();

	newData.Tag = VMDTagEnum::VariantMesh;
	newData.varintMeshData.modelPath = xmlNode.attribute_no_case(VMDTagAtrtibtes::Model).value();
	newData.varintMeshData.imposterModelPath = xmlNode.attribute_no_case(VMDTagAtrtibtes::ImposterModel).value();

	DxMaterialInfoReader(&newData).Parse();

	return nullptr;
}
// TODO: clean up, new node or fill param node?
DxVmdNode::SharedPtr rldx::SlotNodeCreator::Create(DxVmdNode* root, const pugi::xml_node& xmlNode)
{
	//auto newNode = std::make_shared<DxVmdNode>();
	VMDNodeData& vmdNodeData = root->GetVMDNodeDataRef();
	vmdNodeData.Tag = VMDTagEnum::Slot;
	vmdNodeData.Name = xmlNode.attribute_no_case(VMDTagAtrtibtes::Name).as_string();
	vmdNodeData.slotData.name = vmdNodeData.Name;
	vmdNodeData.slotData.attcachPointName = xmlNode.attribute_no_case(VMDTagAtrtibtes::AttachPoint).as_string();

	DxMaterialInfoReader materialReader(&vmdNodeData);
	materialReader.Parse();

	return nullptr;
}

DxVmdNode::SharedPtr rldx::VMDRerenceNodeReator::Create(DxVmdNode* sceneGraphNode, const pugi::xml_node& xmlNode)
{


	std::wstring vmdFilePath = xmlNode.attribute_no_case(VMDTagAtrtibtes::Definition).as_string();
	auto vmdBinary = DxResourceManager::GetCallBackFile(vmdFilePath);
	DxVariantMeshTree vmdCreator;
	vmdCreator.BuildTreeFromVariantMeshDef(vmdBinary, sceneGraphNode);



	return nullptr;
}

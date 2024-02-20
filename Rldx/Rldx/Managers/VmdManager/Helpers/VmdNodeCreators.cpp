#include "VmdNodeCreators.h"

#include "DxMaterialReader.h"

using namespace rldx;

inline DxVmdNode::SharedPtr rldx::VariantMeshNodeCreator::Create(DxVmdNode* root, const pugi::xml_node& xmlNode)
{
	auto newNode = std::make_shared<DxVmdNode>();
	VMDNodeData& newData = newNode->GetVMDNodeDataRef();

	newData.Tag = VMDTagEnum::VariantMesh;
	newData.varintMeshData.modelPath = xmlNode.attribute_no_case(VMDTagAtrtibtes::Model).value();
	newData.varintMeshData.imposterModelPath = xmlNode.attribute_no_case(VMDTagAtrtibtes::ImposterModel).value();

	DxMaterialReader(&newData).Parse();

	return newNode;
}

DxVmdNode::SharedPtr rldx::SlotNodeCreator::Create(DxVmdNode* root, const pugi::xml_node& xmlNode)
{
	auto newNode = std::make_shared<DxVmdNode>();
	VMDNodeData& vmdNodeData = newNode->GetVMDNodeDataRef();
	vmdNodeData.Tag = VMDTagEnum::Slot;
	vmdNodeData.Name = xmlNode.attribute_no_case(VMDTagAtrtibtes::Name).as_string();
	vmdNodeData.slotData.name = vmdNodeData.Name;
	vmdNodeData.slotData.attachPointName = xmlNode.attribute_no_case(VMDTagAtrtibtes::AttachPoint).as_string();

	DxMaterialReader materialReader(&vmdNodeData);
	materialReader.Parse();

	return newNode;
}

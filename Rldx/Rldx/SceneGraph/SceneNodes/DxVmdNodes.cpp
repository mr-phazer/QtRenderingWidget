#include "DxDeformerNode.h"
#include "DxVmdNodes.h"


rldx::DxVMDSlotNode::DxVMDSlotNode(const pugi::xml_node& xmlNode)
{
	vmdNodeData.tagType = VMDTagEnum::Slot;
	vmdNodeData.tagName = xmlNode.attribute_no_case(VMDTagAttributes::Name).as_string();
	vmdNodeData.slotData.attcachPointName = xmlNode.attribute(VMDTagAttributes::AttachPoint).as_string();
	vmdNodeData.slotData.probability = xmlNode.attribute(VMDTagAttributes::Probaility).as_float();
}

rldx::DxVMDVariantMeshNode::DxVMDVariantMeshNode(const pugi::xml_node& xmlNode)
{
	vmdNodeData.tagType = VMDTagEnum::VariantMesh;
	vmdNodeData.tagName = xmlNode.attribute_no_case(VMDTagAttributes::Name).as_string();
	vmdNodeData.varintMeshData.modelPath = xmlNode.attribute_no_case(VMDTagAttributes::Model).value();
	vmdNodeData.varintMeshData.imposterModelPath = xmlNode.attribute_no_case(VMDTagAttributes::ImposterModel).value();

	auto parent = dynamic_cast<DxVmdNode*>(GetParent());

	if (!parent) return;

	// set the node's attach point name from the node's parent, as all children of a <SLOT> attach shall be attached
	vmdNodeData.slotData.attcachPointName = parent->vmdNodeData.slotData.attcachPointName;
}
#include "DxDeformerNode.h"
#include "DxVmdNodes.h"

namespace rldx
{

	rldx::DxVMDSlotNode::DxVMDSlotNode(const pugi::xml_node& xmlNode)
	{
		SetTypeString(L"DxVmdSlotNode");
		vmdNodeData.tagType = VMDTagEnum::Slot;
		vmdNodeData.tagName = xmlNode.attribute_no_case(VMDTagAttributes::Name).as_string();
		vmdNodeData.slotData.attcachPointName = xmlNode.attribute(VMDTagAttributes::AttachPoint).as_string();
		vmdNodeData.slotData.probability = xmlNode.attribute(VMDTagAttributes::Probaility).as_float();
	}

	rldx::DxVMDVariantMeshNode::DxVMDVariantMeshNode(const pugi::xml_node& xmlNode)
	{
		SetTypeString(L"DxVMDVariantMeshNode");
		vmdNodeData.tagType = VMDTagEnum::VariantMesh;
		vmdNodeData.tagName = xmlNode.attribute_no_case(VMDTagAttributes::Name).as_string();
		vmdNodeData.varintMeshData.modelPath = xmlNode.attribute_no_case(VMDTagAttributes::Model).value();
		vmdNodeData.varintMeshData.imposterModelPath = xmlNode.attribute_no_case(VMDTagAttributes::ImposterModel).value();
	}
}
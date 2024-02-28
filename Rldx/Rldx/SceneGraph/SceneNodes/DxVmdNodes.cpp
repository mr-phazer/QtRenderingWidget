#include "DxVmdNodes.h"

rldx::DxVMDSlotNode::DxVMDSlotNode(const pugi::xml_node& xmlNode, std::vector<std::wstring>& destAttachPointNames)
{
	VMDTagData().tagType = VMDTagEnum::Slot;
	VMDTagData().tagName = xmlNode.attribute_no_case(VMDTagAttributes::Name).as_string();
	VMDTagData().slotData.attcachPointName = xmlNode.attribute(VMDTagAttributes::AttachPoint).as_string();
	VMDTagData().slotData.probality = xmlNode.attribute(VMDTagAttributes::Probaility).as_float();
}

rldx::DxVMDVariantMeshNode::DxVMDVariantMeshNode(const pugi::xml_node& xmlNode)
{
	VMDTagData().tagType = VMDTagEnum::VariantMesh;
	VMDTagData().tagName = xmlNode.attribute_no_case(VMDTagAttributes::Name).as_string();
	VMDTagData().varintMeshData.modelPath = xmlNode.attribute_no_case(VMDTagAttributes::Model).value();
	VMDTagData().varintMeshData.imposterModelPath = xmlNode.attribute_no_case(VMDTagAttributes::ImposterModel).value();
}



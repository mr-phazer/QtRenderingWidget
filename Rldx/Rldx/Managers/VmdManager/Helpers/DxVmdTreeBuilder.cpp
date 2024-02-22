#include "DxVmdTreeBuilder.h"

using namespace rldx;
using namespace rmv2;

void rldx::DxVmdTreeBuilder::Build(DxVmdNode* sceneGraphNode, const pugi::xml_node& xmlNode)
{	
	if (CompareNoCase(xmlNode.name(), VMDTag::VariantMesh))
	{
		VariantMeshNodeCreator variantMeshNodeCreator;
		variantMeshNodeCreator.Create(sceneGraphNode, xmlNode);
	}
	else if (CompareNoCase(xmlNode.name(), VMDTag::Slot))
	{
		SlotNodeCreator().Create(sceneGraphNode, xmlNode);		
	}
	else if (CompareNoCase(xmlNode.name(), VMDTag::VariantMeshReference))
	{		
		VMDRerenceNodeReator().Create(sceneGraphNode, xmlNode);		
	}
	else
	{	
		sceneGraphNode->GetVMDNodeDataRef().Name = xmlNode.attribute_no_case(VMDTagAtrtibtes::Name).as_string();	
	}

	for (auto& iiVarianMeshsNode : xmlNode.children())	{
		
		auto newSceneNode = std::make_shared<DxVmdNode>(); // new node for each child node
		sceneGraphNode->AddChild(newSceneNode);
		Build(newSceneNode.get(), iiVarianMeshsNode);		
	}
}
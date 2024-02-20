#include "DxVmdTreeBuilder.h"


using namespace rldx;
using namespace rmv2;

void rldx::VmdNodeTreeBuilder::Build(DxVmdNode* sceneGraphRoot, const pugi::xml_node& xmlNode)
{
	DxVmdNode::SharedPtr currentNode;
	if (CompareNoCase(xmlNode.name(), VMDTag::VariantMesh))
	{
		VariantMeshNodeCreator variantMeshNodeCreator;
		currentNode = variantMeshNodeCreator.Create(sceneGraphRoot, xmlNode);
		sceneGraphRoot->AddChild(currentNode);

	}
	else if (CompareNoCase(xmlNode.name(), VMDTag::Slot))
	{
		currentNode = SlotNodeCreator().Create(sceneGraphRoot, xmlNode);
		sceneGraphRoot->AddChild(currentNode);

	}

	currentNode = std::make_shared<DxVmdNode>();
	currentNode->GetVMDNodeDataRef().Name = xmlNode.attribute_no_case(VMDTagAtrtibtes::Name).value();
	sceneGraphRoot->AddChild(currentNode);


	for (auto& iiVarianMeshsNode : xmlNode.children())
	{
		Build(currentNode.get(), iiVarianMeshsNode);
	}

}


#pragma once


#include "..\..\..\SceneGraph\SceneNodes\DxVmdNode.h"


namespace rldx
{

class IDxVmdNodeCreator
{
public:
	/// <summary>
	/// Creates a VMD sceneGraphRoot from the XML, with tag, model path, and material
	/// But does not allocate any DX resources
	/// </summary>
	/// <param name="sceneGraphRoot">node to attach ne nodes to</param>
	/// <param name="xmlNode"the xml tag></param>	
	/// <returns>New nodes with children on sucess</returns>
	virtual  DxVmdNode::SharedPtr Create(DxVmdNode* sceneGraphRoot, const pugi::xml_node& xmlNode) = 0;	
};

class VariantMeshNodeCreator : public IDxVmdNodeCreator
{
public:
	virtual DxVmdNode::SharedPtr Create(DxVmdNode* root, const pugi::xml_node& xmlNode) override;
};


class SlotNodeCreator : public IDxVmdNodeCreator
{
public:
	virtual DxVmdNode::SharedPtr Create(DxVmdNode* root, const pugi::xml_node& xmlNode) override;
};
}

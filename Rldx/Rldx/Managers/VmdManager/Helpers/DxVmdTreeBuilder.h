#pragma once

#include "..\..\..\SceneGraph\SceneNodes\DxVmdNodes.h"

namespace rldx {

	class DxDeformerNode;

	class DxVmdTreeBuilder
	{
		std::vector<std::wstring> m_attachPoints;

		// TODO: should this be here, is there any use for it?
		//pugi::xml_document m_xmldoc;

	public:
		void Build(rldx::DxResourceManager& resourceManager, DxVmdNode* sceneGraphRoot, const pugi::xml_node& xmlNode);
	private:
		void LoadVariantMeshRerenceNode(rldx::DxResourceManager& resourceManager, DxVmdNode* sceneGraphNode, const pugi::xml_node& xmlNode);
	};


}
#pragma once

#include "..\..\..\..\..\ImportExport\Helpers\Templates.h"
#include "..\..\..\SceneGraph\SceneNodes\DxVmdNodes.h"
#include "VmdNodeCreators.h"

namespace rldx {

	class DxDeformerNode;

	class DxVmdTreeBuilder
	{
		std::vector<std::wstring> m_attachPoints;

		// TODO: should this be here, is there any use for it?
		//pugi::xml_document m_xmldoc;

	public:
		void Build(DxVmdNode* sceneGraphRoot, const pugi::xml_node& xmlNode);
	private:
		void LoadVariantMeshRerenceNode(DxVmdNode* sceneGraphNode, const pugi::xml_node& xmlNode);
	};


}
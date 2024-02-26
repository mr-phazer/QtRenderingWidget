#pragma once

#include "..\..\..\..\..\ImportExport\Helpers\Templates.h"
#include "..\..\..\SceneGraph\SceneNodes\DxVmdNode.h"
#include "VmdNodeCreators.h"

namespace rldx {

	class DxVmdTreeBuilder
	{
		std::vector<std::wstring> m_attachPoints;
		pugi::xml_document m_xmldoc;

	public:
		void Build(DxVmdNode* sceneGraphRoot, const pugi::xml_node& xmlNode);
		void LoadUnknownNode(rldx::DxVmdNode* sceneGraphNode, const pugi::xml_node& xmlNode);
		void LoadVarintMeshNode(DxVmdNode* root, const pugi::xml_node& xmlNode);
		void LoadSlotNode(DxVmdNode* root, const pugi::xml_node& xmlNode);
		void LoadVariantMeshRerence(DxVmdNode* sceneGraphNode, const pugi::xml_node& xmlNode);
	};
}
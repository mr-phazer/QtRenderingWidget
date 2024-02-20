#pragma once

#include "..\..\SceneGraph\SceneNodes\VariannMeshNode\DxVmdNode.h"
#include "Helpers\DxVmdTreeBuilder.h"
#include <memory>

namespace rldx
{
	/// <summary>
	/// Control class for VariantMeshDef tree
	/// </summary>
	class DxVmdManager
	{
		DxVmdNode::SharedPtr m_vmdRootNode;
		VmdNodeTreeBuilder m_treeBuilder;
		pugi::xml_document m_xmlDoc;
		std::vector<std::wstring> m_attachPointNames;

	public:
		DxVmdNode* GetNode() { return m_vmdRootNode.get(); }


		void Create(ByteStream& bytes)
		{
			auto xmlParseResult = m_xmlDoc.load_buffer(bytes.GetBufferPtr(), bytes.GetBufferSize());

			if (!xmlParseResult) {
				throw std::exception(("Create::Read(): XML error: " + std::string(xmlParseResult.description())).c_str());
			}

			m_vmdRootNode = std::make_shared<DxVmdNode>();


			BuildTree(m_vmdRootNode.get(), m_xmlDoc, m_attachPointNames)

		}

	private:
		void BuildTree(DxVmdNode* sceneGraphRoot, const pugi::xml_node& xmlNode, std::vector < std::wstring >& destAttachPointNames)
		{
			m_treeBuilder.Build(sceneGraphRoot, xmlNode);
		}


	};

}

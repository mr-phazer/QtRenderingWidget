#pragma once

#include "..\..\SceneGraph\SceneNodes\DxVmdNode.h"
#include "Helpers\DxVmdTreeBuilder.h"
#include "Helpers\DxVmdAllocator.h"
#include "..\..\Creators\DxGameShaderCreators.h"
#include "..\..\Managers\DxDeviceManager.h"
#include <memory>

namespace rldx
{
	// TODO: rename+move to creator/factory/builder/service NOT MANAGER?
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
		void Create(ByteStream& bytes)
		{
			auto xmlParseResult = m_xmlDoc.load_buffer(bytes.GetBufferPtr(), bytes.GetBufferSize());

			if (!xmlParseResult) {
				throw std::exception(("Create::Read(): XML error: " + std::string(xmlParseResult.description())).c_str());
			}

			m_vmdRootNode = std::make_shared<DxVmdNode>();

			BuildTree(m_vmdRootNode.get(), m_xmlDoc, m_attachPointNames);			
		}

		void AllocateNodes()
		{
			auto newPbrShaderCreator = GameShaderProgramCreatorFactory().Get(game_id_keys::KEY_WARHAMMER_3);
			if (!newPbrShaderCreator) {
				throw std::exception("No shader program creator found for game");
			}

			ByteStream emptyBytes;
			auto newPbrShaderProgram = newPbrShaderCreator->Create(DxDeviceManager::Device(), emptyBytes);
			if (!newPbrShaderProgram) {
				throw std::exception("Error Creating Shader");
			}

			AllocateNodesRecursive(m_vmdRootNode.get(), newPbrShaderProgram);
		}

		// TODO: implement
		/// <summary>
		/// Reload the entire model without changing anything else
		/// </summary>
		void Refresh() {}

		DxVmdNode::SharedPtr GetNode() { return m_vmdRootNode; }

	private:
		void AllocateNodesRecursive(DxVmdNode* m_vmdRootNode, DxMeshShaderProgram* shaderProgram )
		{
			DxVmdNodeAllocator(m_vmdRootNode, shaderProgram).AllocateDxBuffers();

			for (auto& childNode : m_vmdRootNode->GetChildren())
			{
				AllocateNodesRecursive(static_cast<DxVmdNode*>(childNode.get()), shaderProgram);
			}
		}


		void BuildTree(DxVmdNode* vmdRootNode, const pugi::xml_node& xmlNode, std::vector < std::wstring >& destAttachPointNames)
		{
			m_treeBuilder.Build(vmdRootNode, xmlNode);
		}	

	};
}
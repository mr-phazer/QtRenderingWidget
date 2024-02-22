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
	class DxVariantMeshTree
	{
		DxVmdNode::SharedPtr m_vmdRootNode;
		DxVmdTreeBuilder m_treeBuilder;
		pugi::xml_document m_xmlDoc;
		std::vector<std::wstring> m_attachPointNames;

	public:
		void Create(ByteStream& bytes)
		{
			auto result = CompareExtension(bytes.GetPath(), rldx::FileExtensions::VariantMeshDef);
			if (result)
			{
				BuildTreeFromVariantMeshDef(bytes);
				return;
			}

			if (CompareExtension(bytes.GetPath(), rldx::FileExtensions::RigidModelV2))
			{
				BuildTreeFromRigidModel(bytes);
				return;
			}

			if (CompareExtension(bytes.GetPath(), rldx::FileExtensions::WSmodel))
			{
				BuildTreeFromWsmodel(bytes);
				return;
			}			
		}
		
		void BuildTreeFromRigidModel(ByteStream& bytes)
		{
			std::exception("not implemented");
		}

		void BuildTreeFromWsmodel(ByteStream& bytes)
		{
			std::exception("not implemented");
		}

		void BuildTreeFromVariantMeshDef(ByteStream& bytes)
		{
			auto xmlParseResult = m_xmlDoc.load_buffer(bytes.GetBufferPtr(), bytes.GetBufferSize());

			if (!xmlParseResult) {
				throw std::exception(("Create::Read(): XML error: " + std::string(xmlParseResult.description())).c_str());
			}

			m_vmdRootNode = std::make_shared<rldx::DxVmdNode>();

			auto xmlRoot = m_xmlDoc.first_child();

			BuildTree(m_vmdRootNode.get(), xmlRoot, m_attachPointNames);			
		}

		void BuildTreeFromVariantMeshDef(ByteStream& bytes, DxVmdNode* destSceneNode)
		{
			auto xmlParseResult = m_xmlDoc.load_buffer(bytes.GetBufferPtr(), bytes.GetBufferSize());

			if (!xmlParseResult) {
				throw std::exception(("Create::Read(): XML error: " + std::string(xmlParseResult.description())).c_str());
			}			

			auto xmlRoot = m_xmlDoc.first_child();

			BuildTree(destSceneNode, xmlRoot, m_attachPointNames);		
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

		void EnableVariantMesh(DxVmdNode* node)
		{
			// if the "root" is variant mesh, enable all "SLOTS"
			if (node->GetVMDNodeDataRef().Tag == VMDTagEnum::VariantMesh)
			{
				node->SetDrawState(DxBaseNode::DrawStateEnum::Draw);				
				
				for (auto& child : node->GetChildren()) // -- enable all sibling <SLOT> 
				{		
					auto pVmdChildNode = static_cast<DxVmdNode*>(child.get());
					if (pVmdChildNode->GetVMDNodeDataRef().Tag == VMDTagEnum::Slot)
					{
						EnableVariantMesh(pVmdChildNode);
					}
				}
			}
			else if (node->GetVMDNodeDataRef().Tag == VMDTagEnum::Slot) // if the "root" is SLOT, enable 1 variantmesh
			{
				node->SetDrawState(DxBaseNode::DrawStateEnum::Draw);

				if (node->GetChildCount()>0) // enable only 1 variantmesh per slow
				{
					auto vmdNode = static_cast<DxVmdNode*>(node->GetChild(0));
					EnableVariantMesh(vmdNode);
				}
				
				//
				//for (auto& child : node->GetChildren())
				//{
				//	auto vmdNode = static_cast<DxVmdNode*>(child.get()); // only 1 
				//	if (vmdNode->GetVMDNodeDataRef().Tag == VMDTagEnum::VariantMesh)
				//	{
				//		EnableVariantMesh(vmdNode);
				//		break;
				//	}
				//}
			}
		}

		void EnableSlot(DxVmdNode* node)
		{
			if (node->GetVMDNodeDataRef().Tag == VMDTagEnum::Slot) {
				node->SetDrawState(DxBaseNode::DrawStateEnum::Draw);
			}

			for (auto& child : node->GetChildren())
			{
				auto vmdNode = static_cast<DxVmdNode*>(child.get());
				if (vmdNode->GetVMDNodeDataRef().Tag == VMDTagEnum::VariantMesh)
				{
					EnableVariantMesh(vmdNode);
				}
			}
		}



		void GenerateVariant()
		{
			EnableVariantMesh(m_vmdRootNode.get());
		}


		// TODO: implement
		/// <summary>
		/// Reload the entire model without changing anything else
		/// </summary>
		void Refresh() {}

		DxVmdNode::SharedPtr GetNode() { return m_vmdRootNode; }

	private:
		void AllocateNodesRecursive(DxVmdNode* m_vmdRootNode, DxMeshShaderProgram* shaderProgram)
		{
			DxVmdNodeAllocator(m_vmdRootNode, shaderProgram).AllocateDxBuffers();

			for (auto& childNode : m_vmdRootNode->GetChildren())
			{
				AllocateNodesRecursive(static_cast<DxVmdNode*>(childNode.get()), shaderProgram);
			}
		}


		void BuildTree(DxVmdNode* vmdRootNode, const pugi::xml_node& xmlDocument, std::vector < std::wstring >& destAttachPointNames)
		{
			m_treeBuilder.Build(vmdRootNode, xmlDocument);
		}

	};
}
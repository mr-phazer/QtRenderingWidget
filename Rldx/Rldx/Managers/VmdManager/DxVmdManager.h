#pragma once

#include <memory>
#include "..\..\Managers\VmdManager\Helpers\DxVmdTreeBuilder.h"
#include "..\..\SceneGraph\SceneNodes\DxVariantMeshNode.h"
#include "..\..\SceneGraph\SceneNodes\DxVmdNodes.h"

namespace rldx
{
	// TODO: rename+move to creator/factory/builder/service NOT MANAGER?
	/// <summary>
	/// Control class for VariantMeshDef tree
	/// </summary>
	class DxVmdManager
	{
		rldx::DxResourceManager* m_resourceManager;
		std::wstring m_skeletonName;
		DxVmdNode* m_vmdRootNode = nullptr;
		DxBaseNode* m_sceneRootNode = nullptr;
		DxVariantMeshNode* m_poVariantMeshNode = nullptr;

		std::unique_ptr<DxDeformerNode> m_deformerNode; // the skeleton animating this VMD

		// TODO: these are stored ONLY for later saving? With no saving they could just be instantiated locally
		DxVmdTreeBuilder m_treeBuilder;
		pugi::xml_document m_xmlDoc;

		// TODO: does this need to be a state?
		std::vector<std::wstring> m_attachPointNames;

	public:

		DxVmdManager(rldx::DxResourceManager& resourceManager) : m_resourceManager(&resourceManager) {}

		void LoadVariantMeshIntoNode(DxBaseNode* poSceneNode, utils::ByteStream& bytes, const std::wstring& gameIdString);

		void GenerateNewVariant();
		DxDeformerNode* GetDerformer();

	private:
		// TODO: convers the rmv2.file.header to "wstring" for consistency?
		// TODO: is it ok that method both allocated, and fetches skeleton? (2 jobs for 1 methods)
		void AllocateTreeDXBuffers(const std::wstring& gameIdString, WStringkeyMap<sm::Matrix>& preTransformMap);
		void AllocateNodeDXBufferRecursive(rldx::DxResourceManager& resourceManager, DxVmdNode* m_sceneRootNode, DxMeshShaderProgram* shaderProgram, WStringkeyMap<sm::Matrix>& preTransformMap);

		/// <summary>
		/// Loads the skeleton bind pose, etc
		/// </summary>
		void InitDeformation();

		/// <summary>
		/// Setup-s the mesh deformation by a skeleton
		/// </summary>
		void SetMeshDeformation(DxBaseNode* vmdNode);

		/// <summary>
		/// Sets up individual "models nodes"(like a weapons) being moved by 1 bone (attach point=
		/// </summary>		
		void SetAttachPointsRecursive(DxMeshNode* vmdNode);

		void BuildTreeFromAssetFile(utils::ByteStream& bytes);
		void LoadFromRigidModel(utils::ByteStream& bytes);
		void LoadFromWsmodelXML(utils::ByteStream& bytes);
		void LoadFromVmdXML(utils::ByteStream& bytes);
	};
}



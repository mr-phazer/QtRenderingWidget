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
		std::wstring m_skeletonName;
		std::shared_ptr<DxVmdNode> m_vmdRootNode;
		DxBaseNode* m_sceneRootNode = nullptr;
		std::unique_ptr<DxDeformerNode> m_deformerNode; // the skeleton animating this VMD
		std::shared_ptr<DxVariantMeshNode> m_variantMeshNode;

		// TODO: these are stored ONLY for later saving? With no saving they could just be instantiated locally
		DxVmdTreeBuilder m_treeBuilder;
		pugi::xml_document m_xmlDoc;

		// TODO: does this need to be a state?
		std::vector<std::wstring> m_attachPointNames;



	public:
		void LoadVariantMesh(
			DxBaseNode* poSceneNode,
			ByteStream& bytes,
			const std::wstring& gameIdString);


		void GetNewVariant();

		DxDeformerNode* GetDerformer();

		// TODO: implement; 
		/// <summary>
		/// Reload the entire model without changing anything else
		/// </summary>
		virtual void Refresh()
		{

			// TODO: this algo:
			/*
				erase tree
				Remake with the same "root" VMD/RMV2/WSMODEL
				Run AllocateTreeDXBuffers
			*/
		}

		DxVmdNode::SharedPtr GetNode();

	private:
		// TODO: convers the rmv2.file.header to "wstring" for consistency?
		// TODO: is it ok that method both allocated, and fetches skeleton? (2 jobs for 1 methods)
		void AllocateTreeDXBuffers(const std::wstring& gameIdString, WStringkeyMap<sm::Matrix>& preTransformMap);
		void AllocateNodeDXBufferRecursive(DxVmdNode* m_vmdRootNode, DxMeshShaderProgram* shaderProgram, WStringkeyMap<sm::Matrix>& preTransformMap);

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

		void BuildTreeFromXml(ByteStream& bytes);
		void LoadFromRigidModel(ByteStream& bytes);
		void LoadFromWsmodelXML(ByteStream& bytes);
		void LoadFromVmdXML(ByteStream& bytes);
	};
}



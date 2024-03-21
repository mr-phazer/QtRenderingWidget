#pragma once

#include <memory>
#include "..\..\Creators\DxGameShaderCreators.h"

#include "..\..\Managers\DxDeviceManager.h"
#include "..\..\Managers\VmdManager\Helpers\DxMaterialInfoReader.h"
#include "..\..\Managers\VmdManager\Helpers\DxVmdAllocator.h"
#include "..\..\Managers\VmdManager\Helpers\DxVmdTreeBuilder.h"
#include "..\..\Managers\VmdManager\Helpers\VariantGenerator.h"
#include "..\..\SceneGraph\SceneNodes\DxVmdNodes.h"

namespace rldx
{
	// TODO: rename+move to creator/factory/builder/service NOT MANAGER?
	/// <summary>
	/// Control class for VariantMeshDef tree
	/// </summary>
	class DxVmdManager
	{
		DxBaseNode::SharedPtr m_rootNode;
		DxVmdNode::SharedPtr m_vmdNode;
		std::shared_ptr<DxDeformerNode> m_deformerNode; // the skeleton animating this VMD
		DxVmdTreeBuilder m_treeBuilder;
		pugi::xml_document m_xmlDoc;
		std::vector<std::wstring> m_attachPointNames;

	public:
		void LoadVariantMesh(DxBaseNode* poSceneNode, ByteStream& bytes, const std::wstring& gameIdString);



		void LoadAnimation(const std::wstring& animPath);
		void GenerateVariant();

		// TODO: implement
		/// <summary>
		/// Reload the entire model without changing anything else
		/// </summary>
		virtual void Refresh()
		{
			// TODO: this algo:
			/*
				erase tree
				Remake with the same "root" VMD/RMV2/WSMODEL
				Run AllocateDXBuffers
			*/
		}

		DxVmdNode::SharedPtr GetNode();

	private:
		// TODO: convers the rmv2.file.header to "wstring" for consistency?
		// TODO: is it ok that method both allocated, and fetches skeleton? (2 jobs for 1 methods)
		void AllocateDXBuffers(const std::wstring& gameIdString, std::string& destSkeletonName, WStringkeyMap<sm::Matrix>& preTransformMap);
		void AllocateNodesRecursive(DxVmdNode* m_vmdRootNode, DxMeshShaderProgram* shaderProgram, std::string& destSkeletonName, WStringkeyMap<sm::Matrix>& preTransformMap);

		/// <summary>
		/// Loads the skeleton bind pose, etc
		/// </summary>
		void LoadSkeleton(DxBaseNode* poSceneNode, std::string& skeletonName);

		/// <summary>
		/// Setups the mesh deformation by a skeleton
		/// </summary>
		void SetMeshDeformation(DxBaseNode* vmdNode);

		/// <summary>
		/// Sets up individual "models nodes"(like a weapons) being moved by 1 bone (attach point=
		/// </summary>		
		void SetAttachPoints(DxMeshNode* vmdNode);

		void BuildTreeFromXml(ByteStream& bytes);
		void LoadFromRigidModel(ByteStream& bytes);
		void LoadFromWsmodelXML(ByteStream& bytes);
		void LoadFromVmdXML(ByteStream& bytes);
	};
}
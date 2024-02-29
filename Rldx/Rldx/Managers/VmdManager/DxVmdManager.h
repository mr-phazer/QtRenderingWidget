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
		DxVmdNode::SharedPtr m_vmdRootNode;
		DxVmdTreeBuilder m_treeBuilder;
		pugi::xml_document m_xmlDoc;
		std::vector<std::wstring> m_attachPointNames;
		ByteStream m_vmdhBytes;

	public:
		void LoadVariantMesh(ByteStream& bytes);
		void AllocateDXBuffers(const std::wstring& gameIdString);
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
		void LoadFromRigidModel(ByteStream& bytes);
		void LoadFromWsmodelXML(ByteStream& bytes);
		void LoadFromVmdXML(ByteStream& bytes);

		void AllocateNodesRecursive(DxVmdNode* m_vmdRootNode, DxMeshShaderProgram* shaderProgram);;
	};
}
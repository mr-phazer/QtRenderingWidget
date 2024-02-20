#pragma once

#include "..\..\..\..\..\ImportExport\Helpers\Templates.h"
#include "..\..\..\SceneGraph\SceneNodes\DxVmdNode.h"
#include "VmdNodeCreators.h"

namespace rldx
{
	class DxVmdNodeAllocator
	{		
		DxVmdNode* m_sceneVmdNode = nullptr;
		DxMeshShaderProgram* m_nodeShaderProgram = nullptr;

	public:
		DxVmdNodeAllocator() = delete;
		DxVmdNodeAllocator(DxVmdNode* in, DxMeshShaderProgram* nodeShaderProgram) : m_sceneVmdNode(in), m_nodeShaderProgram(nodeShaderProgram) {};

		void AllocateDxBuffers();

	private:
		void AllocateVariantMesh();		
	};
	
	//class DxVmdNodeAllocator
	//{				
	//public:
	//	void AllocateDxBuffers(DxVmdNode* m_sceneVmdNode);

	//private:
	//	void AllocateVariantMesh(DxVmdNode* m_sceneVmdNode);
	//};
}
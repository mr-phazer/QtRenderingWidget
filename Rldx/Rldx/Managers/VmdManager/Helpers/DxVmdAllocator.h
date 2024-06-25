#pragma once

#include "..\..\..\SceneGraph\SceneNodes\DxVmdNodes.h"

namespace rldx
{
	class DxDeformerNode;
	//TODO: this does not need to a class with states??
	class DxVmdNodeAllocator
	{
		DxVmdNode* m_sceneVmdNode = nullptr;
		DxMeshShaderProgram* m_nodeShaderProgram = nullptr;

	public:
		DxVmdNodeAllocator() = delete;
		DxVmdNodeAllocator(DxVmdNode* in, DxMeshShaderProgram* nodeShaderProgram) : m_sceneVmdNode(in), m_nodeShaderProgram(nodeShaderProgram) {};

		void AllocateDxBuffers(std::wstring & destSkeletonName, WStringkeyMap<sm::Matrix>& preTransformMap);

	private:
		void AllocateVariantMesh(std::wstring & wdestSkeletonName, WStringkeyMap<sm::Matrix>& pretransformMap);
	};

	//class DxVmdNodeAllocator
	//{				
	//public:
	//	void AllocateDxBuffers(DxVmdNode* m_sceneVmdNode);

	//private:
	//	void AllocateVariantMesh(DxVmdNode* m_sceneVmdNode);
	//};
}
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
		rldx::DxResourceManager* m_resourceManager;

	public:
		DxVmdNodeAllocator() = delete;
		DxVmdNodeAllocator(rldx::DxResourceManager& resourceManager, DxVmdNode* in, DxMeshShaderProgram* nodeShaderProgram)
			:
			m_resourceManager(&resourceManager),
			m_sceneVmdNode(in),
			m_nodeShaderProgram(nodeShaderProgram) {};

		void AllocateDxBuffers(std::wstring & destSkeletonName, utils::WStringkeyMap<sm::Matrix>& preTransformMap);

	private:
		void AllocateVariantMesh(std::wstring & wdestSkeletonName, utils::WStringkeyMap<sm::Matrix>& pretransformMap);
	};

	//class DxVmdNodeAllocator
	//{				
	//public:
	//	void AllocateDxBuffers(DxVmdNode* m_sceneVmdNode);

	//private:
	//	void AllocateVariantMesh(DxVmdNode* m_sceneVmdNode);
	//};
}

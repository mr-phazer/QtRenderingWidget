#pragma once

#include "DXSwapChain.h"
#include "..\SceneGraph\SceneGraph.h"

namespace Rldx {

	struct DxScene
	{
	public:
		DxSceneNode* GetRootNode();
		void DeleteNode(DxSceneNode* node);

	private:
		DxSwapChain m_swapChain;
		DxSceneNode::SPtr m_spoRootNode = DxSceneNode::Create("RootNode");
	};

}; // end: namespace Rldx
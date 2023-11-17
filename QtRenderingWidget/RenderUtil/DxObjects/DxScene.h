#pragma once

#include "DXSwapChain.h"
#include "..\SceneGraph\SceneGraph.h"

namespace Rldx {

	struct DxScene
	{

	public:
		SceneNode* GetRootNode();;
		void DeleteNode(SceneNode* node);

	private:
		SceneNode::SPtr m_spoRootNode = SceneNode::Create("RootNode");
	};

}; // end: namespace Rldx
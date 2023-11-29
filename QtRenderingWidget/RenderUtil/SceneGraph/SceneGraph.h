#pragma once

#include "SceneNodes/DxSceneNode.h"

namespace Rldx
{
	class SceneGraph
	{
	public:
		DxSceneNode* GetRootNode();

	private:
		std::shared_ptr<DxSceneNode> m_spoRootNode;
	};



};

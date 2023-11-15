#pragma once

#include "SceneNodes/SceneNode.h"

namespace Rldx
{
	class SceneGraph
	{
	public:
		SceneNode* GetRootNode();
		
	};
}




private:
	std::shared_ptr<SceneNode> m_spoRootNode;
};

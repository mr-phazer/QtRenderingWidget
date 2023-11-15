#pragma once

#include "..\SceneGraph\SceneNodes\SceneNode.h"

namespace Rldx {

	class DxSceneManager
	{
		public:

		SceneNode* GetRootNode();
		
		void DeleteNode(SceneNode* node);

	private:
		SceneNode::SPtr m_spoRootNode = BaseSceneNode::Create("RootNode");
	};


}; // namespace Rldx

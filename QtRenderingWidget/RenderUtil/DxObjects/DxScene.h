#pragma once

#include "DXSwapChain.h"
#include "..\SceneGraph\SceneGraph.h"

namespace Rldx {

	struct DxScene
	{




		SceneGraph m_sceneGraph;
		DxSwapChain::SPtr m_spoSwapChain;
	};

}; // end: namespace Rldx
#pragma once
#include "IDxModelNodeCreator.h"

namespace rldx
{
	class DxWSModelModelnodeCretor : public IDxModelNodeCreator
	{
		DxModelNode::SharedPtr CreateNode(ID3D11Device* poDevice, DxBaseNode* rootNode, ByteStream& data) override;
	};
}
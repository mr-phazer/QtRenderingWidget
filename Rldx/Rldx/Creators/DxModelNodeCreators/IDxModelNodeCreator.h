#pragma once
struct ID3D11Device;
class ByteStream;

#include "..\..\SceneGraph\SceneNodes\DxModelNode.h"

namespace rldx {
	class DxScene;
	class DxVmdNode;

	class IDxModelNodeCreator
	{
	public:
		virtual DxModelNode::SharedPtr CreateNode(ID3D11Device* poDevice, DxBaseNode* rootNode, ByteStream& data) = 0;
		//virtual DxModelNode::SharedPtr AlloNode(ID3D11De	vice* poDevice, DxBaseNode* rootNode, ByteStream& data) = 0;

	};


}

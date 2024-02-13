#pragma once
#include "IDxModelNodeCreator.h"

namespace rldx
{
    class DxRmv2ModelNodeCreator :  public IDxModelNodeCreator
    {
    public:
        virtual DxModelNode::SharedPtr CreateNode(ID3D11Device* poDevice, DxBaseNode* rootNode, ByteStream& data) override;        
    };

}

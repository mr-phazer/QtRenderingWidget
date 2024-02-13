#include "DxRmv2ModelNodeCreator.h"
#include "..\..\ImportExport\RigidModel\Readers\RigidModelReader.h"

using namespace rldx;
using namespace rmv2;

DxModelNode::SharedPtr DxRmv2ModelNodeCreator::CreateNode(ID3D11Device* poDevice, DxBaseNode* rootNode, ByteStream& fileData)
{
	RigidModelReader rigidModelFileReader;
	auto rmv2File = rigidModelFileReader.Read(fileData);

	auto modelNodeRmv2 = DxNodeCreator::CreateNode<DxModelNode>("ModelNode RMV2");
	modelNodeRmv2->SetModelData(poDevice, rmv2File);
	modelNodeRmv2->LoadMaterialDataFromRmv2(poDevice, rmv2File);

	return modelNodeRmv2;
}

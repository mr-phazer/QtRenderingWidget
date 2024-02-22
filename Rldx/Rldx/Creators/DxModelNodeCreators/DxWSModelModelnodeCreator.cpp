#include "DxWSModelModelnodeCreator.h"

#include "..\..\ImportExport\Libs\PugiXML\pugixml.hpp"
#include "..\..\ImportExport\WsModel\Reader\WsModelReader.h"
#include "..\..\ImportExport\RigidModel\Readers\RigidModelReader.h"


using namespace rldx;
using namespace rmv2;

DxModelNode::SharedPtr rldx::DxWSModelModelnodeCretor::CreateNode(ID3D11Device* poDevice, DxBaseNode* rootNode, ByteStream& data)
{
	auto wsModelData = rmv2::WsModelReader().Read(data);
	auto bytestreamFileData = DxResourceManager::GetCallBackFile(wsModelData.geometryPath);

	auto rmv2File = RigidModelReader().Read(bytestreamFileData);

	auto modelNodeRmv2 = DxNodeCreator::CreateNode<DxModelNode>("ModelNode RMV2");
	modelNodeRmv2->SetModelData(poDevice, rmv2File);
	modelNodeRmv2->LoadMaterialDataFromRmv2 (poDevice, rmv2File);

	return modelNodeRmv2;


	//auto xmlLoadResult = m_wsmodelFile.load_buffer(data.GetBufferPtr(), data.GetBufferSize());
	//pugi::xml_document wsmodelFile;

	//if (!xmlLoadResult)	{

	//	std::string errprDescrion  = xmlLoadResult.description();
	//	throw std::exception(("error parsing WSMODEL" + errprDescrion).c_str());
	//}


	//return DxModelNode::SharedPtr();
}

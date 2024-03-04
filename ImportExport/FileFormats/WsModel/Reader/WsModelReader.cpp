#include "WsModelReader.h"


#include "..\..\..\FileFormats\XmMaterial\Readers\XmMaterialReader.h"
#include "..\..\..\Helpers\CustomExceptions.h"
#include "..\..\Rldx\Rldx\Tools\tools.h"

using namespace std;
using namespace rmv2;

WsModelData WsModelReader::Read(ByteStream& fileData)
{
	LoadXML(fileData);

	auto xmlModel = m_xmlFile.child_no_case(L"model");

	GetRMVPath(xmlModel);
	ReadMaterialPaths(xmlModel);

	return m_wsmodelData;
}

void WsModelReader::ReadMaterialPaths(const pugi::xml_node& xmlModel)
{
	auto xmlMaterialList = xmlModel.child_no_case(L"Materials");
	auto xmlMaterialItem = xmlMaterialList.child_no_case(L"mAterial");

	while (xmlMaterialItem) // run throughh the <material> tags
	{
		// get the "lod/part" indexes (lod, mesh)
		size_t part_index = xmlMaterialItem.attribute_no_case(L"part_index").as_uint();
		size_t lod_index = xmlMaterialItem.attribute_no_case(L"lod_index").as_uint();

		// add data chunk to 2d lod/part vector
		XMLMaterialData materialdata;
		materialdata.xmlMaterialFilePath = xmlMaterialItem.text().as_string();

		ReadXMLMaterialFiles(materialdata);

		m_wsmodelData.AddXmlMaterial(materialdata, lod_index, part_index);

		xmlMaterialItem = xmlMaterialItem.next_sibling();
	}
}

void rmv2::WsModelReader::ReadXMLMaterialFiles(XMLMaterialData& materialdata)
{

	XmMaterialReader xmlMaterialReader(&materialdata);
	xmlMaterialReader.Read();
}

void WsModelReader::GetRMVPath(const pugi::xml_node& xmlModel)
{
	// TODO: remove this line, when no_case worksW
	//auto xmlGeomtry = get_child_nocase(xmlModel, L"geometry");
	auto xmlGeomtry = xmlModel.child_no_case(L"Geometry");
	m_wsmodelData.geometryPath = xmlGeomtry.text().as_string();
}

void WsModelReader::LoadXML(ByteStream& fileData)
{
	auto parseResult = m_xmlFile.load_buffer(fileData.GetBufferPtr(), fileData.GetBufferSize());

	if (!parseResult)
	{
		// TODO: remove these two lines if the custom exception works well
		//std::string errorText = parseResult.description();
		//throw std::exception(("Error Parsing WSModel File: " + libtools::wstring_to_string(fileData.GetPath()) + ", Decsription:" + errorText).c_str());
		throw XMLException(parseResult);
	}
}


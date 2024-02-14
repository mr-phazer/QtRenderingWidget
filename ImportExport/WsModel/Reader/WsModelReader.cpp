#include "WsModelReader.h"

#include "..\..\XmMaterial\Readers\XmMaterialReader.h"
#include "..\..\Helpers\CustomExceptions.h"

using namespace std;
using namespace rmv2;

pugi::xml_attribute get_attribute_nocase(const pugi::xml_node& root, const std::wstring& attrName)
{
	// find first tag <model> in case there is XML header crap before
	return root.find_attribute(
		[&](pugi::xml_attribute& attr)
		{
			return (toLower(attr.name()) == toLower(attrName));
		}
	);
}

pugi::xml_node get_child_nocase(const pugi::xml_node& root, const std::wstring& childName)
{
	int i = 1;
	// find first tag <model> in case there is XML header crap before
	return root.find_child(
		[&](pugi::xml_node& node)
		{
			return (toLower(node.name()) == toLower(childName));
		}
	);
}

rmv2::WsModelData rmv2::WsModelReader::Read(ByteStream& fileData)
{
	LoadXML(fileData);	
	
	auto xmlModel = get_child_nocase(m_xmlFile, L"model");	

	GetRMVPath(xmlModel);
	ReadMaterialPaths(xmlModel);

	return m_wsmodelData;
}

std::vector<TextureElement> rmv2::WsModelReader::GetTexturesFromXmlMaterial(size_t lodIndex, size_t partIndex)
{
	if (lodIndex >= m_wsmodelData.xmlMateriData.size())
	{
		throw std::exception("GetTexturesForMaterial: LOD index is out of bounbds");
	}

	if (partIndex >= m_wsmodelData.xmlMateriData[lodIndex].size())
	{
		throw std::exception("GetTexturesForMaterial: Mesh(part) indexi is out of bounds.");
	}

	return m_wsmodelData.xmlMateriData[lodIndex][partIndex].textures;
}

void rmv2::WsModelReader::ReadMaterialPaths(const pugi::xml_node & xmlModel)
{
	auto xmlMaterialList = xmlModel.child_no_case(L"Materials");
	auto xmlMaterialItem = xmlMaterialList.child_no_case(L"mAterial");

	while (xmlMaterialItem) // run throughh the <material> tags
	{
		auto part_index = get_attribute_nocase(xmlMaterialItem, L"part_index").as_uint();
		auto lod_index = get_attribute_nocase(xmlMaterialItem, L"lod_index").as_uint();
		
		auto& materialData = m_wsmodelData.AllocXmLMaterial(lod_index, part_index);
		materialData.xmlMaterialFilePath = xmlMaterialItem.text().get();

		rmv2::XmMaterialReader(&materialData).Read();

		xmlMaterialItem = xmlMaterialItem.next_sibling();
	}
}

void rmv2::WsModelReader::GetRMVPath(const pugi::xml_node & xmlModel)
{ 
	// TODO: remove this line, when no_case worksW
	//auto xmlGeomtry = get_child_nocase(xmlModel, L"geometry");
	auto xmlGeomtry = xmlModel.child_no_case(L"Geometry");
	m_wsmodelData.geometryPath = xmlGeomtry.text().as_string();
}

void rmv2::WsModelReader::LoadXML(ByteStream& fileData)
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


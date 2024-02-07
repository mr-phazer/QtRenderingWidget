#include "WsModelReader.h"

pugi::xml_attribute get_attribute_nocase(pugi::xml_node& root, const std::wstring& attrName)
{
	// find first tag <model> in case there is XML header crap before
	return root.find_attribute(
		[&](pugi::xml_attribute& attr)
		{
			return (toLower(attr.name()) == toLower(attrName));
		}
	);
}

pugi::xml_node get_child_nocase(pugi::xml_node& root, const std::wstring& attrName)
{

	int i = 1;
	// find first tag <model> in case there is XML header crap before
	return root.find_child(
		[&](pugi::xml_node& node)
		{
			return (toLower(node.name()) == toLower(attrName));
		}
	);
}

rmv2::WsModelReader rmv2::WsModelReader::Create(ByteStream& fileData)
{
	WsModelReader newInstance;
	auto parseResult = newInstance.xmlFile.load_buffer(fileData.GetBufferPtr(), fileData.GetBufferSize());

	if (!parseResult) {
		throw std::exception(string("Error Parsing WSModel File: " + narrow(fileData.GetPath())).c_str());
	}

	return newInstance;
}

rmv2::WsModelData rmv2::WsModelReader::Read()
{
	WsModelData wsModelData;
	auto xmlModel = get_child_nocase(xmlFile, L"model");

	// TODO: parse <Geomtry>
	// TODO: parse <materials>, use "AddXmlMaterialPath()"

	return wsModelData;
}

void rmv2::WsModelReader::AddXmlMaterialPath(WsModelData& wsmodelData, const std::wstring& xmlMaterialFilePath, size_t lodIndex, size_t partIndex)
{
	// resize 2d vectors as needed
	if (lodIndex >= wsmodelData.xmlMateriFilePaths.size()) {
		wsmodelData.xmlMateriFilePaths.resize(lodIndex + 1);
	}

	if (partIndex >= wsmodelData.xmlMateriFilePaths[lodIndex].size()) {
		wsmodelData.xmlMateriFilePaths[lodIndex].resize(partIndex + 1);
	}

	wsmodelData.xmlMateriFilePaths[lodIndex][partIndex] = xmlMaterialFilePath;
}

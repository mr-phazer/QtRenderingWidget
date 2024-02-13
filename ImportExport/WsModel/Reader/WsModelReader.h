#pragma once

#ifndef PUGIXML_WCHAR_MODE
#define PUGIXML_WCHAR_MODE
#endif

#include "..\..\Libs\PugiXML\pugixml.hpp"
#include "..\..\Helpers\ByteStream.h"
#include "..\DataTypes\WsModelData.h"
#include "..\..\XmMaterial\Readers\XmMaterialReader.h"


extern pugi::xml_attribute get_attribute_nocase(const pugi::xml_node& root, const std::wstring& attrName);;
extern pugi::xml_node get_child_nocase(const  pugi::xml_node& root, const std::wstring& attrName);

namespace rmv2
{
	class WsModelReader
	{
		pugi::xml_document m_xmlFile;
		WsModelData m_wsmodelData;

	public:
		WsModelData Read(ByteStream& fileData);

		std::vector<TextureElement> GetTexturesFromXmlMaterial(size_t lodIndex, size_t partIndex);

	private:
		XMLMaterialData& AllocXmLMaterial(size_t lodIndex, size_t partIndex);;
		void ReadMaterialPaths(const pugi::xml_node& xmlModel);
		void GetRMVPath(const pugi::xml_node& xmlModel);
		void LoadXML(ByteStream& fileData);
	};
}

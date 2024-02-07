#pragma once

#ifndef PUGIXML_WCHAR_MODE
#define PUGIXML_WCHAR_MODE
#endif

#include "..\..\Libs\PugiXML\pugixml.hpp"
#include "..\..\Helpers\ByteStream.h"
#include "..\DataTypes\WsModelData.h"

extern pugi::xml_attribute get_attribute_nocase(pugi::xml_node& root, const std::wstring& attrName);;

extern pugi::xml_node get_child_nocase(pugi::xml_node& root, const std::wstring& attrName);


namespace rmv2
{
	class WsModelReader
	{
		pugi::xml_document xmlFile;

	public:
		static WsModelReader Create(ByteStream& fileData);

		WsModelData Read();

		static void AddXmlMaterialPath(
			WsModelData& wsmodelData,
			const std::wstring& xmlMaterialPath,
			size_t lodIndex,
			size_t partIndex);;
	};
} 
	
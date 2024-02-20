#pragma once

#ifndef PUGIXML_WCHAR_MODE
#define PUGIXML_WCHAR_MODE
#endif

#include "..\..\Libs\PugiXML\pugixml.hpp"
#include "..\..\Helpers\ByteStream.h"
#include "..\DataTypes\WsModelData.h"

namespace rmv2
{
	class WsModelReader
	{
		pugi::xml_document m_xmlFile;
		WsModelData m_wsmodelData;

	public:
		WsModelData Read(ByteStream& fileData);
		//std::vector<TextureElement> GetTexturesFromXmlMaterial(size_t lodIndex, size_t partIndex);

	private:		
		void ReadMaterialPaths(const pugi::xml_node& xmlModel);		
		void GetRMVPath(const pugi::xml_node& xmlModel);
		void LoadXML(ByteStream& fileData);
		void ReadXMLMaterialFiles(XMLMaterialData& materialdata);
	};
}

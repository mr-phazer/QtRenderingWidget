#pragma once

#include "Utils\ByteStream.h"
#include "..\..\..\Libs\PugiXML\pugixml.hpp"
#include "..\DataTypes\WsModelData.h"

namespace rmv2
{
	using namespace utils;

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

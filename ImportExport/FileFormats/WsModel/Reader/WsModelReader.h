#pragma once

#include "..\..\..\Libs\PugiXML\pugixml.hpp"
#include "..\DataTypes\WsModelData.h"
#include "Utils\ByteStream.h"

namespace rmv2
{
	class WsModelReader
	{
		pugi::xml_document m_xmlFile;
		WsModelData m_wsmodelData;

	public:
		WsModelData Read(utils::ByteStream& fileData);
		//std::vector<TextureElement> GetTexturesFromXmlMaterial(size_t lodIndex, size_t partIndex);

	private:
		void ReadMaterialPaths(const pugi::xml_node& xmlModel);
		void GetRMVPath(const pugi::xml_node& xmlModel);
		void LoadXML(utils::ByteStream& fileData);
		void ReadXMLMaterialFiles(XMLMaterialData& materialdata);
	};
}

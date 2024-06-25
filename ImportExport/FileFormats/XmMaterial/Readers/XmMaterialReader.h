#pragma once

#include "..\..\..\Libs\PugiXML\pugixml.hpp"
#include "..\DataTypes\XMLMaterialData.h"


namespace rmv2
{
	class XmMaterialReader
	{
		XMLMaterialData* m_poMaterialData;
		pugi::xml_document m_xmlDoc;

		XmMaterialReader() {};
	public:
		XmMaterialReader(XMLMaterialData* pData) : m_poMaterialData(pData) {};
		XMLMaterialData Read();


	private:
		void FetchTextures();
	};
}
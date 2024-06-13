#include "..\..\..\Libs\PugiXML\pugixml.hpp"
#include "..\..\Rldx\Rldx\Managers\ResourceManager\DxResourceManager.h"
#include "XmMaterialReader.h"

#include <map>

using namespace rmv2;
using namespace rldx;
using namespace utils;


bool CmpList(const std::wstring& in, const std::vector<std::wstring>& values)
{
	for (auto& str : values)
	{
		if (ToUpper(str) == ToUpper(in))
			return true;
	}

	return false;
}

XMLMaterialData rmv2::XmMaterialReader::Read()
{
	if (!m_poMaterialData) {
		throw std::exception("INTERNAL ERROR: XmMaterialReader: supplied data == nullptr");
	}

	auto bytes = rldx::DxResourceManager::GetCallBackFile(m_poMaterialData->xmlMaterialFilePath);

	auto xmlParseResult = m_xmlDoc.load_buffer(bytes.GetBufferPtr(), bytes.GetBufferSize());

	if (!xmlParseResult) {
		throw std::exception(("XMlXmMaterialReader::Read(): XML error: " + std::string(xmlParseResult.description())).c_str());
	}

	FetchTexturesFromXML();

	return *m_poMaterialData;
}
// TODO: add error checking, for invalid XML nodes, anything that can crash
// TODO: make this more neat

void rmv2::XmMaterialReader::FetchTexturesFromXML()
{
	std::map < TextureTypeEnum, std::vector<std::wstring>> mapTypeToNames =
	{
		{eDiffuse,			{ L"s_diffuse", L"diffuse" }},
		{eGlossMap,			{ L"s_gloss", L"gloss" }},
		{eSpecular,			{ L"s_specular", L"specular"  }},
		{eNormal,			{ L"s_normal", L"normal", L"t_xml_normal",  L"s_xml_normal" } },
		{eBaseColor,		{ L"t_xml_base_colour", L"s_xml_base_colour" }},
		{eMaterialMap,		{ L"t_xml_material_map", L"s_xml_material_map" }},
		{eMask,				{ L"s_mask", L"t_xml_mask" L"s_xml_mask" }},
	};

	auto xmlMaterial = m_xmlDoc.child_no_case(L"material");
	auto xmlTextureList = xmlMaterial.child_no_case(L"textures");
	auto xmlTextureElement = xmlTextureList.child_no_case(L"texture");

	while (xmlTextureElement)
	{
		auto xmlSlot = xmlTextureElement.child_no_case(L"slot");
		auto xmlSource = xmlTextureElement.child_no_case(L"source");
		std::wstring slotName = xmlSlot.text().get();
		std::wstring slotPath = xmlSource.text().get();

		for (auto& mapItem : mapTypeToNames)
		{
			if (CmpList(slotName, mapItem.second))
			{
				m_poMaterialData->AddTexture(mapItem.first, ToString(slotPath));
			}
		}

		xmlTextureElement = xmlTextureElement.next_sibling();
	}
}

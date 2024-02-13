#include "XmMaterialReader.h"
#include "..\..\Rldx\Rldx\Managers\ResourceManager\DxResourceManager.h"
#include "..\..\Libs\PugiXML\pugixml.hpp"

#include <map>

using namespace rmv2;
using namespace rldx;


bool CmpList(const std::wstring& in, const std::vector<std::wstring>& values)
{
	for (auto& str : values)
	{
		if (toUpper(str) == toUpper(in))
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

void rmv2::XmMaterialReader::FetchTexturesFromXML()
{
	// TODO: add error checking, for invalid XML nodes, anything that can crash
	// TODO: make this more neat
	std::map < TextureTypeEnum, std::vector<std::wstring>> mapTypeToNames =
	{
		{eDiffuse,			{ L"s_diffuse", L"diffuse" }},
		{eGlossMap,			{ L"s_gloss", L"gloss" }},
		{eSpecular,			{ L"s_specular", L"specular"  }},
		{eNormal,			{ L"s_normal", L"normal", L"t_xml_normal",  L"s_xml_normal" } },
		{eBaseColor,		{ L"t_xml_base_colour", L"s_xml_base_colour" }},
		{eMaterialMap,		{ L"t_xml_material_map, s_xml_material_map" }},
		{eMask,				{ L"s_mask", L"t_xml_mask" L"s_xml_mask" }},
	};

	const std::vector<std::wstring> diffuseNames = { L"s_diffuse",	L"diffuse" };
	const std::vector<std::wstring> specularNames = { L"s_specular", L"specular" };
	const std::vector<std::wstring> glossNames = { L"s_gloss", L"gloss" };
	const std::vector<std::wstring> normalNames = { L"s_normal", L"normal", L"t_xml_normal",  L"s_xml_normal" };
	const std::vector<std::wstring> baseColorNames = { L"t_xml_base_colour", L"s_xml_base_colour" };
	const std::vector<std::wstring> materialMapNames = { L"t_xml_material_map, s_xml_material_map" };
	const std::vector<std::wstring> maskNames = { L"s_mask", L"t_xml_mask" L"s_xml_mask" };

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
				m_poMaterialData->AddTexture(mapItem.first, libtools::wstring_to_string(slotPath));
			}
		}


		//if (CmpList(slotName, diffuseNames))
		//{
		//	m_poMaterialData->AddTexture(TextureTypeEnum::eDiffuse, libtools::wstring_to_string(slotPath));
		//}
		//else if (CmpList(slotName, specularNames))
		//{
		//	m_poMaterialData->AddTexture(TextureTypeEnum::eSpecular, libtools::wstring_to_string(slotPath));
		//}
		//else if (CmpList(slotName, normalNames))
		//{
		//	m_poMaterialData->AddTexture(TextureTypeEnum::eNormal, libtools::wstring_to_string(slotPath));
		//}
		//else if (CmpList(slotName, baseColorNames))
		//{
		//	m_poMaterialData->AddTexture(TextureTypeEnum::eBaseColor, libtools::wstring_to_string(slotPath));
		//}
		//else if (CmpList(slotName, materialMapNames))
		//{
		//	m_poMaterialData->AddTexture(TextureTypeEnum::eMaterialMap, libtools::wstring_to_string(slotPath));
		//}
		//else if (CmpList(slotName, maskNames))
		//{
		//	m_poMaterialData->AddTexture(TextureTypeEnum::eMask, libtools::wstring_to_string(slotPath));
		//}
		//// TODO: finish, withh all the textures, weird names, from all games

		xmlTextureElement = xmlTextureElement.next_sibling();
	}
}

#include <string>
#include "DxMaterial.h"
#include "DxTexture.h"

#include "..\Managers\DxDeviceManager.h"

using namespace utils;
using namespace rldx;

DxMaterial* rldx::DxMaterial::Create(std::vector<rmv2::TextureElement>& textures, rldx::DxResourceManager& resourceManager)
{
	auto newMaterial = resourceManager.CreateResouce<DxMaterial>();
	newMaterial->InitWithDefaulTextures(resourceManager); // fill with default textures first, so if any texures are missing, the model will sill draw

	newMaterial->SetTextures(DxDeviceManager::Device(), resourceManager, textures);
	return newMaterial;
}

void rldx::DxMaterial::InitWithDefaulTextures(rldx::DxResourceManager& resourceManager)
{
	// TODO: make sure these are deallocate when SetTextures is called for the same texture type id
	m_textures[TextureTypeEnum::eBaseColor] = { DxTexture::GetTextureFromFile(resourceManager, L"default_base_colour.dds") };
	m_textures[TextureTypeEnum::eDiffuse] = { DxTexture::GetTextureFromFile(resourceManager, L"default_grey.dds") };
	m_textures[TextureTypeEnum::eSpecular] = { DxTexture::GetTextureFromFile(resourceManager, L"default_specular.dds") };
	m_textures[TextureTypeEnum::eGlossMap] = { DxTexture::GetTextureFromFile(resourceManager, L"default_gloss_map.dds") };
	m_textures[TextureTypeEnum::eMaterialMap] = { DxTexture::GetTextureFromFile(resourceManager, L"default_material_map.dds") };
	m_textures[TextureTypeEnum::eNormal] = { DxTexture::GetTextureFromFile(resourceManager, L"default_normal.dds") };
}

bool DxMaterial::operator==(const DxMaterial& other) const
{
	return m_pathHash == other.m_pathHash;
}

void rldx::DxMaterial::SetTextures(ID3D11Device* poDevice, rldx::DxResourceManager& resourceManager, const std::vector<rmv2::TextureElement>& inTex)
{
	for (auto& itText : inTex)
	{
		AddTexture(poDevice, resourceManager, itText.textureType, ToWString(itText.texturePath));

		m_pathHash.insert(std::strlen(m_pathHash.c_str()), itText.texturePath);
		m_pathHash += '\r';
	}
}

DxMaterial* DxMaterial::Create(ID3D11Device* poDevice, DxResourceManager& resourceManager, const std::vector<InputTextureElement>& textures)
{
	auto newMaterial = resourceManager.CreateResouce<DxMaterial>();

	for (auto& tex : textures)
	{
		newMaterial->AddTexture(poDevice, resourceManager, tex.type, tex.path);
	};

	return new DxMaterial();
}

inline bool IsDDSTextureFile(char* bin)
{
	return
		(bin[0] == 'D') &&
		(bin[1] == 'D') &&
		(bin[2] == 'S');
}

// TODO: current not used, as all slots are filled with default textures, remove?
inline bool IsTextureCriticalForMaterial(const std::wstring& filePath)
{
	return !(
		(ToLower(filePath).find(L"mask") != std::wstring::npos)
		);
}

void DxMaterial::AddTexture(ID3D11Device* poDevice, DxResourceManager& resourceManager, UINT slot, const std::wstring& path)
{
	DxTexture* textPtr = nullptr;

	textPtr = resourceManager.CreateResouce<DxTexture>();

	logging::LogAction(L"DEBUG: attempting to get 1 file from CALLBACK: " + path);

	auto bytes = DxResourceManager::GetFile(path);

	if (!bytes.IsValid()) // texture file is missing or empty
	{
		logging::LogError(L"Loading From CALLBACK failed, missing or empty file, " + path);

		return;
	}

	textPtr->LoadFileFromMemory(poDevice, bytes.GetBufferPtr(), bytes.GetBufferSize(), path);

	logging::LogAction(L"Loaded From CALLBACK: " + path);

	// Destroy existing texture (for example, the pre set default texture)
	resourceManager.DestroyResource(m_textures[TextureTypeEnum(slot)].pTexture);

	m_textures[TextureTypeEnum(slot)] = { textPtr };
}


DxTexture* DxMaterial::LoadDefaultTexture(rldx::DxResourceManager& resourceManager, ID3D11Device* poDevice, UINT slot)
{
	try {
		auto resHandleDiffuse = rldx::DxTexture::GetTextureFromFile(resourceManager, m_defaultTexturesMap.at(TextureTypeEnum(slot)));
		return resHandleDiffuse;
	}
	catch (std::exception& e) {
		auto debug_1 = e.what(); // TODO: REMOVE!!
		logging::LogAction(L"Failed to load default texture: key enum id: " + ToWString(std::to_string(slot)) + L", message: " + ToWString(e.what()));
		return nullptr;
	}

	// TODO: remove if above works

	//switch (TextureTypeEnum(slot))
	//{
	//case TextureTypeEnum::eBaseColor:
	//{
	//	auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_grey.dds");
	//	return resHandleDiffuse;
	//}

	//case TextureTypeEnum::eMaterialMap:
	//{
	//	auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_metal_material_map.dds");
	//	return resHandleDiffuse;
	//}

	//case TextureTypeEnum::eDiffuse:
	//{
	//	auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_grey.dds");
	//	return resHandleDiffuse;
	//}

	//case TextureTypeEnum::eGlossMap:
	//{
	//	auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_gloss_map.dds");
	//	return resHandleDiffuse;
	//}

	//case TextureTypeEnum::eSpecular:
	//{
	//	auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_grey.dds");
	//	return resHandleDiffuse;
	//}

	//case TextureTypeEnum::eNormal:
	//{
	//	auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_normal.dds");
	//	return resHandleDiffuse;
	//}

	//default:
	//{
	//	auto resHandleDefaultGrey = DxResourceManager::Instance()->GetTexture(L"default_grey.dds");
	//	return resHandleDefaultGrey;
	//}
	//}
}

void DxMaterial::BindToDC(ID3D11DeviceContext* poDeviceContext)
{
	for (auto& itTextureInfo : m_textures)
	{
		poDeviceContext->PSSetShaderResources(
			GetTextureStartSlot() + itTextureInfo.first,
			1,
			(itTextureInfo.second.pTexture != nullptr) ? itTextureInfo.second.pTexture->GetAddressOfShaderResourceView() : nullptr
		);
	}
}

void DxMaterial::UnbindFromDC(ID3D11DeviceContext* poDC)
{
	// TODO: TEST THHIS:
	poDC->PSSetShaderResources(GetTextureStartSlot(), (UINT)m_textures.size(), m_emptyMaterial.data());
}

int DxMaterial::GetTextureStartSlot()
{
	return 47;
}

std::vector<ID3D11ShaderResourceView*> DxMaterial::m_emptyMaterial = std::vector<ID3D11ShaderResourceView*>(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, nullptr);
#include "DxMaterial.h"
#include "DxTexture.h"

#include <string>

#include "..\Managers\DxDeviceManager.h"

using namespace rldx;

DxMaterial* rldx::DxMaterial::Create(std::vector<rmv2::TextureElement>& textures)
{
	auto newMaterial = rldx::DxResourceManager::Instance()->AllocMaterial().GetPtr();
	newMaterial->InitWithDefaulTextures();

	newMaterial->SetTextures(DxDeviceManager::Device(), textures);

	return newMaterial;
}

void rldx::DxMaterial::InitWithDefaulTextures()
{
	m_textures[TextureTypeEnum::eBaseColor] = { DxResourceManager::Instance()->GetTexture(L"default_base_colour.dds").GetPtr() };
	m_textures[TextureTypeEnum::eDiffuse] = { DxResourceManager::Instance()->GetTexture(L"default_grey.dds").GetPtr() };
	m_textures[TextureTypeEnum::eSpecular] = { DxResourceManager::Instance()->GetTexture(L"default_specular.dds").GetPtr() };
	m_textures[TextureTypeEnum::eGlossMap] = { DxResourceManager::Instance()->GetTexture(L"default_gloss_map.dds").GetPtr() };
	m_textures[TextureTypeEnum::eMaterialMap] = { DxResourceManager::Instance()->GetTexture(L"default_material_map.dds").GetPtr() };
	m_textures[TextureTypeEnum::eNormal] = { DxResourceManager::Instance()->GetTexture(L"default_normal.dds").GetPtr() };
}

bool DxMaterial::operator==(const DxMaterial& other) const
{
	return m_pathHash == other.m_pathHash;
}

void rldx::DxMaterial::SetTextures(ID3D11Device* poDevice, const std::vector<rmv2::TextureElement>& inTex)
{
	for (auto& itText : inTex)
	{
		AddTexture(poDevice, itText.textureType, ToWString(itText.texturePath));
	}
}

DxMaterial* DxMaterial::Create(ID3D11Device* poDevice, const std::vector<InputTextureElement>& textures)
{
	auto newMaterial = DxResourceManager::Instance()->AllocMaterial().GetPtr();

	for (auto& tex : textures)
	{
		newMaterial->AddTexture(poDevice, tex.type, tex.path);
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

void DxMaterial::AddTexture(ID3D11Device* poDevice, UINT slot, const std::wstring& path)
{
	DxTexture* textPtr = nullptr;

	textPtr = DxResourceManager::Instance()->AllocTexture().GetPtr();

	Logger::LogAction("DEBUG: attempting to get 1 file from CALLBACK: " + path);

	auto bytes = DxResourceManager::GetFile(path);

	if (!bytes.IsValid()) // texture file is missing or empty
	{
		Logger::LogActionError(L"Loading From CALLBACK failed, missing or empty file, " + path);

		return;
	}

	textPtr->LoadFileFromMemory(poDevice, bytes.GetBufferPtr(), bytes.GetBufferSize());

	Logger::LogActionSuccess(L"Loaded From CALLBACK: " + path);

	m_textures[TextureTypeEnum(slot)] = { textPtr };
}


DxTexture* DxMaterial::LoadDefaultTexture(ID3D11Device* poDevice, UINT slot)
{
	try {
		auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(m_defaultTexturesMap.at(TextureTypeEnum(slot)));
		return resHandleDiffuse.GetPtr();
	}
	catch (std::exception& e) {
		auto debug_1 = e.what(); // TODO: REMOVE!!
		Logger::LogActionError(L"Failed to load default texture: key enum id: " + ToWString(std::to_string(slot)) + L", message: " + ToWString(e.what()));
		return nullptr;
	}

	// TODO: remove if above works

	//switch (TextureTypeEnum(slot))
	//{
	//case TextureTypeEnum::eBaseColor:
	//{
	//	auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_grey.dds");
	//	return resHandleDiffuse.GetPtr();
	//}

	//case TextureTypeEnum::eMaterialMap:
	//{
	//	auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_metal_material_map.dds");
	//	return resHandleDiffuse.GetPtr();
	//}

	//case TextureTypeEnum::eDiffuse:
	//{
	//	auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_grey.dds");
	//	return resHandleDiffuse.GetPtr();
	//}

	//case TextureTypeEnum::eGlossMap:
	//{
	//	auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_gloss_map.dds");
	//	return resHandleDiffuse.GetPtr();
	//}

	//case TextureTypeEnum::eSpecular:
	//{
	//	auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_grey.dds");
	//	return resHandleDiffuse.GetPtr();
	//}

	//case TextureTypeEnum::eNormal:
	//{
	//	auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_normal.dds");
	//	return resHandleDiffuse.GetPtr();
	//}

	//default:
	//{
	//	auto resHandleDefaultGrey = DxResourceManager::Instance()->GetTexture(L"default_grey.dds");
	//	return resHandleDefaultGrey.GetPtr();
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
			itTextureInfo.second.pTexture->GetAddressOfShaderResourceView()
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

ResourceTypeEnum DxMaterial::GetType() const
{
	return ResourceTypeEnum::Material;
}

std::wstring DxMaterial::GetTypeString() const
{
	return L"DxMaterial";
}


std::vector<ID3D11ShaderResourceView*> DxMaterial::m_emptyMaterial = std::vector<ID3D11ShaderResourceView*>(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, nullptr);
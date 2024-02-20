#include "DxMaterial.h"
#include "DxTexture.h"

#include "..\Managers\DxDeviceManager.h"

using namespace rldx;

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
		AddTexture(poDevice, itText.textureType, libtools::string_to_wstring(itText.texturePath));
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

inline bool IsTextureCriticalForMaterial(const wstring& filePath)
{
	return !(
		(toLower(filePath).find(L"mask") != std::wstring::npos)
		);
}

void DxMaterial::AddTexture(ID3D11Device* poDevice, UINT slot, const std::wstring& path)
{
	DxTexture* textPtr = nullptr;

	textPtr = DxResourceManager::Instance()->AllocTexture().GetPtr();

	logging::LogAction("DEBUG: attempting to get 1 file from CALLBACK: " + libtools::wstring_to_string(path));

	auto bytes = DxResourceManager::GetCallBackFile(path);

	if (!bytes.IsValid()) // texture file is missing or empty
	{
		logging::LogActionError("Loading From CALLBACK failed, missing or empty file, " + libtools::wstring_to_string(path));

		return;
	}

	textPtr->LoadFileFromMemory(poDevice, bytes.GetBufferPtr(), bytes.GetBufferSize());

	logging::LogActionSuccess("Loaded From CALLBACK: " + libtools::wstring_to_string(path));

	m_textures[TextureTypeEnum(slot)] = { textPtr };
}





DxTexture* DxMaterial::LoadDefaultTexture(ID3D11Device* poDevice, UINT slot)
{
	switch (TextureTypeEnum(slot))
	{
	case TextureTypeEnum::eBaseColor:
	{
		auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_grey.dds");
		return resHandleDiffuse.GetPtr();
	}

	case TextureTypeEnum::eMaterialMap:
	{
		auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_metal_material_map.dds");
		return resHandleDiffuse.GetPtr();
	}

	case TextureTypeEnum::eDiffuse:
	{
		auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_grey.dds");
		return resHandleDiffuse.GetPtr();
	}

	case TextureTypeEnum::eGlossMap:
	{
		auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_gloss_map.dds");
		return resHandleDiffuse.GetPtr();
	}

	case TextureTypeEnum::eSpecular:
	{
		auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_grey.dds");
		return resHandleDiffuse.GetPtr();
	}

	case TextureTypeEnum::eNormal:
	{
		auto resHandleDiffuse = DxResourceManager::Instance()->GetTexture(L"default_normal.dds");
		return resHandleDiffuse.GetPtr();
	}

	default:
	{
		auto resHandleDefaultGrey = DxResourceManager::Instance()->GetTexture(L"default_grey.dds");
		return resHandleDefaultGrey.GetPtr();
	}

	}

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

std::string DxMaterial::GetTypeString() const
{
	return "DxMaterial";
}

#include "DxMaterial.h"
#include "DxTexture.h"

#include "..\Managers\DxDeviceManager.h"

using namespace rldx;

bool DxMaterial::operator==(const DxMaterial& other) const
{
	return m_pathHash == other.m_pathHash;
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

static bool IsDDSTextureFile(char* bin)
{
	return
		(bin[0] == 'D') &&
		(bin[1] == 'D') &&
		(bin[2] == 'S');
}


void DxMaterial::AddTexture(ID3D11Device* poDevice, UINT slot, const std::wstring& path)
{
	DxTexture* textPtr = nullptr;

	if ((textPtr = DxResourceManager::Instance()->GetResourceByString<DxTexture>(path)) != nullptr)
	{
		// TODO: REMOVE and clean up block
		auto DEBUG_1 = 1;
	}
	else
	{
		textPtr = DxResourceManager::Instance()->AllocTexture().GetPtr();

		logging::LogAction("DEBUG: attempting to get 1 file from CALLBACK: " + libtools::wstring_to_string(path));

		auto bytes = DxResourceManager::GetCallBackFile(path);

		if (!bytes.IsValid()) {
			logging::LogActionError("Loading From CALLBACK failed, missing or empty file, (will load default)" + libtools::wstring_to_string(path));
						

			textPtr = LoadDefaultTexture(poDevice, slot);
		}
		else {
			textPtr->LoadFileFromMemory(poDevice, bytes.GetBufferPtr(), bytes.GetBufferSize());
			
			logging::LogActionSuccess("Loaded From CALLBACK: " + libtools::wstring_to_string(path));			
		}

		
	}

	m_textures.push_back({ slot, textPtr });
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
			GetTextureStartSlot() + itTextureInfo.slot,
			1,
			itTextureInfo.pTexture->GetAddressOfShaderResourceView()
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

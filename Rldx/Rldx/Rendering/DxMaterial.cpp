#include "DxMaterial.h"
#include "DxTexture.h"

#include "..\Managers\DxDeviceManager.h"

bool rldx::DxMaterial::operator==(const DxMaterial& other) const
{
	return m_pathHash == other.m_pathHash;
}

rldx::DxMaterial* rldx::DxMaterial::Create(ID3D11Device* poDevice, const std::vector<InputTextureElement>& textures)
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


void rldx::DxMaterial::AddTexture(ID3D11Device* poDevice, UINT slot, const std::wstring& path)
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
			logging::LogActionError("Loading From CALLBACK failed, missing or empty file" + libtools::wstring_to_string(path));
		}
		else {
			logging::LogActionSuccess("Loaded From CALLBACK: " + libtools::wstring_to_string(path));
		}

		textPtr->LoadFileFromMemory(poDevice, bytes.GetBufferPtr(), bytes.GetBufferSize());
	}

	m_textures.push_back({ slot, textPtr });
}

void rldx::DxMaterial::BindToDC(ID3D11DeviceContext* poDeviceContext)
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

int rldx::DxMaterial::GetTextureStartSlot()
{
	return 47;
}

rldx::ResourceTypeEnum rldx::DxMaterial::GetType() const
{
	return ResourceTypeEnum::Material;
}

std::string rldx::DxMaterial::GetTypeString() const
{
	return "DxMaterial";
}

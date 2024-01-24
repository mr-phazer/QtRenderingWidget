#include "DxMaterial.h"
#include "DxTexture.h"

#include "..\Managers\DxDeviceManager.h"

// TODO: which implementation to keep?
//inline void rldx::DxMaterial::BindToDC(ID3D11DeviceContext* poDc)
//{
//
//	for (auto& tex : m_textures)
//	{		
//		poDc->PSSetShaderResources(tex.slot, 1, tex.pTexture->GetAddressOfShaderResourceView());
//	}
//
//
//}

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

void rldx::DxMaterial::AddTexture(ID3D11Device* poDevice, UINT slot, const std::wstring& path)
{
	auto textPtr = DxResourceManager::Instance()->AllocTexture().GetPtr();

	textPtr->LoadFile(poDevice, path);

	m_textures.push_back({ slot, textPtr });
}

void rldx::DxMaterial::BindToDC(ID3D11DeviceContext* poDeviceContext)
{
	for (auto& tex : m_textures)
	{
		poDeviceContext->PSSetShaderResources(tex.slot+47, 1, tex.pTexture->GetAddressOfShaderResourceView());
	}
}

rldx::ResourceTypeEnum rldx::DxMaterial::GetType() const
{
	return ResourceTypeEnum::Material;
}

std::string rldx::DxMaterial::GetTypeString() const
{
	return "DxMaterial";
}

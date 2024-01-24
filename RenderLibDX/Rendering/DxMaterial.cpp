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

void rldx::DxMaterial::BindToDC(ID3D11DeviceContext* poDeviceContext)
{
	for (auto& tex : m_textures)
	{
		poDeviceContext->PSSetShaderResources(tex.slot, 1, tex.pTexture->GetAddressOfShaderResourceView());
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

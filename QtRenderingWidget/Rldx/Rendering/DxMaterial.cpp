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

void rldx::DxMaterial::BindToDC(ID3D11DeviceContext* poDC)
{
	auto samplerLinearWrap = rldx::DxDeviceManager::GetInstance().GetDxStates().LinearWrap();
	poDC->PSSetSamplers(0, 1, &samplerLinearWrap);

	for (auto& tex : m_textures)
	{
		tex.pTexture->BindAsShaderResourceView(poDC, tex.slot);
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

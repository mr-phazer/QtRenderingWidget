#include "DxMaterial.h"
#include "DxTexture.h"

#include "..\Managers\DxDeviceManager.h"

// TODO: which implementation to keep?
//inline void Rldx::DxMaterial::Bind(ID3D11DeviceContext* poDc)
//{
//
//	for (auto& tex : m_textures)
//	{		
//		poDc->PSSetShaderResources(tex.slot, 1, tex.pTexture->GetAddressOfShaderResourceView());
//	}
//
//
//}

inline void Rldx::DxMaterial::Bind(ID3D11DeviceContext* poDC)
{
	auto samplerLinearWrap = Rldx::DxDeviceManager::GetInstance().GetDxStates().LinearWrap();
	poDC->PSSetSamplers(0, 1, &samplerLinearWrap);

	for (auto& tex : m_textures)
	{
		tex.pTexture->BindAsShaderResourceView(poDC, tex.slot);
	}
}

Rldx::ResourceTypeEnum Rldx::DxMaterial::GetType() const
{
	return ResourceTypeEnum::Material;
}

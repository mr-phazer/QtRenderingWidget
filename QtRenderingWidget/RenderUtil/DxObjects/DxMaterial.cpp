#include "DxMaterial.h"
#include "DxTextureView.h"

inline void Rldx::DxMaterial::Bind(ID3D11DeviceContext* poDc)
{

	for (auto& tex : textures)
	{
		auto pSRV = tex.texture->getGetShaderResourceView();
		poDc->PSSetShaderResources(tex.channel, 1, &pSRV);
	}


}

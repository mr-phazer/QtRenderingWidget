#include "DxAmbientLight.h"
#include "DxTexture.h"




int Rldx::DxAmbientLight::SetTextures(
	ID3D11Device* poDevice,
	const std::wstring& pathDiffuseMap, 
	const std::wstring& pathSpecularMap, 
	const std::wstring& pathLUT,
	UINT startSlotSRV)
{
	m_poDiffuseMap->LoadFile(poDevice, pathDiffuseMap);
	m_poSpecularMap->LoadFile(poDevice, pathSpecularMap);
	m_poLUT->LoadFile(poDevice, pathLUT);

	return 0;
}

void Rldx::DxAmbientLight::BindToDC(ID3D11DeviceContext* poDC)
{
	// TODO: test/finish this
	ID3D11ShaderResourceView* ppShaderResourceViews[3] = {
		m_poDiffuseMap->GetShaderResourceView(),
		m_poSpecularMap->GetShaderResourceView(),
		m_poLUT->GetShaderResourceView(),
	};

	poDC->PSSetShaderResources(m_startSlotSRV, 3, ppShaderResourceViews);
};


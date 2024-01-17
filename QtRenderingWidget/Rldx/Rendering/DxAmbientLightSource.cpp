#include "DxAmbientLightSource.h"
#include "DxTexture.h"


int rldx::DxAmbientLightSource::SetTextures(
	ID3D11Device* poDevice,
	const std::wstring& pathDiffuseMap, 
	const std::wstring& pathSpecularMap, 
	const std::wstring& pathLUT,
	UINT startSlotSRV)
{
	if (pathDiffuseMap.empty())
		throw invalid_argument("path for IBL Diffuse cubemap is empty");

	if (pathSpecularMap.empty())
		throw invalid_argument("path for IBL Specular cubemap is empty");	

	m_poDiffuseMap->LoadFile(poDevice, pathDiffuseMap);
	m_poSpecularMap->LoadFile(poDevice, pathSpecularMap);
	
	// TODO: if needed make an impl thata loads an LUT
	/*if (!pathLUT.empty()) 
		m_poLUT->LoadFile(poDevice, pathLUT);*/

	return 0;
}

void rldx::DxAmbientLightSource::BindToDC(ID3D11DeviceContext* poDC)
{
	m_poDiffuseMap->BindAsShaderResourceView(poDC, m_startSlotSRV);
	m_poSpecularMap->BindAsShaderResourceView(poDC, m_startSlotSRV+1);

	// TODO: if needed make an impl thata sets an LUT
	//m_poLUT->BindAsShaderResourceView(poDC, m_startSlotSRV+3);

	// TODO: test/finish this
	ID3D11ShaderResourceView* ppShaderResourceViews[2] = {
		m_poDiffuseMap->GetShaderResourceView(),
		m_poSpecularMap->GetShaderResourceView(),
		//m_poLUT->GetShaderResourceView(),
	};

	poDC->PSSetShaderResources(m_startSlotSRV, 3, ppShaderResourceViews);
};


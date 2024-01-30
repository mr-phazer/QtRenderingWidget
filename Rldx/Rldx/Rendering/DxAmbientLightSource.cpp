#include "DxAmbientLightSource.h"


#include "..\Managers\ResourceManager\DxResourceManager.h"
#include "..\Rendering\DxTexture.h"
#include "..\..\ImportExport\Helpers\ByteStream.h"

using namespace rldx;
DxAmbientLightSource rldx::DxAmbientLightSource::Create(ID3D11Device* poDevice, const std::wstring& pathDiffuseMap, const std::wstring& pathSpecularMap, const std::wstring& pathLUT, UINT startSlotSRV, UINT startSlotConstBuf)
{
	DxAmbientLightSource oNewInstance;
	oNewInstance.SetTexturesFromFiles(poDevice, pathDiffuseMap, pathSpecularMap, pathLUT, startSlotSRV);
	oNewInstance.m_oPSConstBuffer.buffer.Create(poDevice);	

	return oNewInstance;
}

DxAmbientLightSource rldx::DxAmbientLightSource::Create(ID3D11Device* poDevice, ByteStream& pathDiffuseMap, ByteStream& pathSpecularMap, ByteStream& pathLUT, UINT startSlotSRV, UINT startSlotConstBuf)
{
	DxAmbientLightSource oNewInstance;
	oNewInstance.SetTexturesFromMemory(poDevice, pathDiffuseMap, pathSpecularMap, pathLUT, startSlotSRV);		
	oNewInstance.m_oPSConstBuffer.buffer.Create(poDevice);	

	return oNewInstance;
}

void rldx::DxAmbientLightSource::SetTexturesFromFiles(
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

	m_poDiffuseMap->LoadFileFromDisk(poDevice, pathDiffuseMap);
	m_poSpecularMap->LoadFileFromDisk(poDevice, pathSpecularMap);

	m_oPSConstBuffer.buffer.Create(poDevice);

	// TODO: if needed make an impl thata loads an LUT
	/*if (!pathLUT.empty())
		m_poLUT->LoadFileFromDisk(poDevice, pathLUT);*/	
}

void rldx::DxAmbientLightSource::SetTexturesFromMemory(
	ID3D11Device* poDevice, 
	ByteStream& streamDiffuseMap, 
	ByteStream& streamSpecularMap,
	ByteStream& pathLUT, 
	UINT startSlotSRV)
{
	m_poDiffuseMap =
		DxResourceManager::Instance()->AllocEmpty<DxTexture>(L"DiffuseIBL").GetPtr();;
	m_poSpecularMap =
		DxResourceManager::Instance()->AllocEmpty<DxTexture>(L"SpecularIBL").GetPtr();;
	
	// TODO: this is used in some shaders so find a way to condintionally load it in neat way
	//m_poLUT =
	//	DxResourceManager::Instance()->AllocEmpty<DxTexture>(L"LUT").GetPtr();;

	m_poSpecularMap->LoadCubeMap(
		poDevice,
		streamSpecularMap.GetBufferPtr(),
		streamSpecularMap.GetBufferSize(),
		"IBL CubeMap Specular");

	m_poDiffuseMap->LoadCubeMap(
		poDevice,
		streamDiffuseMap.GetBufferPtr(),
		streamDiffuseMap.GetBufferSize(),
		"IBL CubeMcap Diffuse");
	

	m_oPSConstBuffer.buffer.Create(poDevice);
}

void rldx::DxAmbientLightSource::BindToDC(ID3D11DeviceContext* poDC)
{
	m_oPSConstBuffer.BindToDC(poDC);

	// TODO: test/finish this
	ID3D11ShaderResourceView* ppShaderResourceViews[2] = {
		m_poDiffuseMap->GetShaderResourceView(),
		m_poSpecularMap->GetShaderResourceView(),
		//m_poLUT->GetShaderResourceView(),
	};

	poDC->PSSetShaderResources(0, 2, ppShaderResourceViews);
};


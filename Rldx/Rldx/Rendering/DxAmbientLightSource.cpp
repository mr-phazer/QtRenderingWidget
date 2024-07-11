#include "DxAmbientLightSource.h"

#include "..\..\..\DXUT\Core\DXUTmisc.h"
#include "..\Managers\ResourceManager\DxResourceManager.h"
#include "..\Rendering\DxTexture.h"
#include "Utils\ByteStream.h"

using namespace std;
using namespace utils;

namespace rldx
{
	DxAmbientLightSource DxAmbientLightSource::Create(ID3D11Device* poDevice, const std::wstring& pathDiffuseMap,
													  const std::wstring& pathSpecularMap, const std::wstring& pathLUT,
													  UINT startSlotSrv, UINT startSlotConstBuf)
	{
		DxAmbientLightSource oNewInstance;

		oNewInstance.SetTexturesFromFiles(poDevice, pathDiffuseMap, pathSpecularMap, pathLUT, startSlotSrv);

		oNewInstance.m_oPSConstBuffer.Init(poDevice, "PS_AmbientLight_CB");
		DXUT_SetDebugName(oNewInstance.m_oPSConstBuffer.GetBuffer(), psConstBufferName);

		return oNewInstance;
	}

	DxAmbientLightSource DxAmbientLightSource::Create(ID3D11Device* poDevice, DxTexture* diffuseCubemap,
													  DxTexture* specularCubmap, DxTexture* lut, UINT startSlotConstBuf,
													  UINT startSlotSRV)
	{
		DxAmbientLightSource oNewInstance;

		oNewInstance.m_poDiffuseMap = diffuseCubemap;
		oNewInstance.m_poSpecularMap = specularCubmap;

		oNewInstance.m_oPSConstBuffer.Init(poDevice, "PS_AmbientLight_CB");
		DXUT_SetDebugName(oNewInstance.m_oPSConstBuffer.GetBuffer(), psConstBufferName);

		return oNewInstance;
	}

	DxAmbientLightSource DxAmbientLightSource::Create(ID3D11Device* poDevice, ByteStream& pathDiffuseMap,
													  ByteStream& pathSpecularMap, ByteStream& pathLUT, UINT startSlotSRV,
													  UINT startSlotConstBuf)
	{
		DxAmbientLightSource oNewInstance;

		oNewInstance.SetTexturesFromMemory(poDevice, pathDiffuseMap, pathSpecularMap, pathLUT, startSlotSRV);

		oNewInstance.m_oPSConstBuffer.Init(poDevice, "PS_DxAmbientLight_CB");
		DXUT_SetDebugName(oNewInstance.m_oPSConstBuffer.GetBuffer(), psConstBufferName);

		return oNewInstance;
	}

	void DxAmbientLightSource::SetTexturesFromFiles(ID3D11Device* poDevice, const std::wstring& pathDiffuseMap,
													const std::wstring& pathSpecularMap, const std::wstring& pathLUT,
													UINT startSlotSRV)
	{
		if (pathDiffuseMap.empty())
			throw std::exception("path for IBL Diffuse cubemap is empty");

		if (pathSpecularMap.empty())
			throw std::exception("path for IBL Specular cubemap is empty");

		m_poDiffuseMap->LoadFileFromDisk(poDevice, pathDiffuseMap);
		m_poSpecularMap->LoadFileFromDisk(poDevice, pathSpecularMap);

		// TODO: if needed make an impl thata loads an LUT
		/*if (!pathLUT.empty())
			m_poLUT->LoadFileFromDisk(poDevice, pathLUT);*/
	}

	void DxAmbientLightSource::SetTexturesFromMemory(ID3D11Device* poDevice, ByteStream& streamDiffuseMap,
													 ByteStream& streamSpecularMap, ByteStream& pathLUT, UINT startSlotSRV)
	{
		m_poDiffuseMap = DxResourceManager::Instance()->AllocEmpty<DxTexture>(L"DiffuseIBL").GetPtr();
		;
		m_poSpecularMap = DxResourceManager::Instance()->AllocEmpty<DxTexture>(L"SpecularIBL").GetPtr();
		;

		// TODO: this is used in some shaders so find a way to condintionally load it in neat way
		// m_poLUT =
		//	DxResourceManager::Instance()->AllocEmpty<DxTexture>(L"LUT").GetPtr();;

		m_poSpecularMap->LoadCubeMap(poDevice, streamSpecularMap.GetBufferPtr(), streamSpecularMap.GetBufferSize(),
									 "IBL CubeMap Specular");

		m_poDiffuseMap->LoadCubeMap(poDevice, streamDiffuseMap.GetBufferPtr(), streamDiffuseMap.GetBufferSize(),
									"IBL CubeMcap Diffuse");
	}

	void DxAmbientLightSource::BindToDC(ID3D11DeviceContext* poDC)
	{
		m_oPSConstBuffer.SetStartSlot(0);
		m_oPSConstBuffer.BindToDC(poDC);

		ID3D11ShaderResourceView* pSrvDiffuseCubeMap[] = {
			m_poDiffuseMap->GetShaderResourceView(),
		};

		ID3D11ShaderResourceView* pSrvSpecularMap[] = {
			m_poSpecularMap->GetShaderResourceView(),
		};

		poDC->PSSetShaderResources(0, 1, pSrvDiffuseCubeMap);
		poDC->PSSetShaderResources(1, 1, pSrvSpecularMap);
	};
} // namespace rldx

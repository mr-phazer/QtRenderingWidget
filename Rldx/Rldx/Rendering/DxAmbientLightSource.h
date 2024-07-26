#pragma once

#include <commonlibs/utils/ByteStream.h>
#include "..\DataTypes\ConstBuffers\CPUConstBuffers.h"
#include "..\Interfaces\IBindable.h"
#include "DxConstBuffer.h"

#include <d3d11.h>

#include <string>

namespace rldx
{
	class DxResourceManager;
	// TODO: maybe, make this base class of all buffers?
	class IBuffer : public IBindable
	{
		UINT m_startSlot = 0;

	public:
		void SetStartSlot(UINT startSlot)
		{
			m_startSlot = startSlot;
		}
	};

	class DxTexture;

	/// <summary>
	/// Sets 2/3 cubemaps needed for PBR imagebased lightng rendering
	/// </summary>
	class DxAmbientLightSource : public IBindable
	{
		static auto constexpr psConstBufferName = "PS_CB:AmbientLight";

		DxTexture* m_poDiffuseMap = nullptr;
		DxTexture* m_poSpecularMap = nullptr;
		DxTexture* m_poLUT = nullptr;

		UINT m_textureStartSlot = 0;

	public:
		TDxPSShaderConstBuffer<PS_AmbientLight_Data_ConstBuffer> m_oPSConstBuffer;

		void SetStartSlot(UINT startSlot)
		{
			m_textureStartSlot = startSlot;
		}

		void SetLightRadiance(float radiance)
		{
			m_oPSConstBuffer.data.radiance = radiance;
		}

		void SetLightColor(const DirectX::XMFLOAT4& vColor = { 1, 1, 1, 1 })
		{
			m_oPSConstBuffer.data.color = vColor;
		}

		/// <summary>
		/// Bind IBL const buffer, and set cubemap textures
		/// </summary>
		/// <param m_nodeName="poDC"></param>
		void BindToDC(ID3D11DeviceContext* poDC) override;

		static DxAmbientLightSource Create(ID3D11Device* poDevice, const std::wstring& pathDiffuseMap,
										   const std::wstring& pathSpecularMap, const std::wstring& pathLUT = L"",
										   UINT startSlotConstBuf = 0, UINT startSlotSRV = 0);

		static DxAmbientLightSource Create(ID3D11Device* poDevice, DxTexture* diffuseCubemap, DxTexture* specularCubemap,
										   DxTexture* lut = nullptr, UINT startSlotConstBuf = 0, UINT startSlotSRV = 0);

		static DxAmbientLightSource Create(ID3D11Device* poDevice, rldx::DxResourceManager& resourceManager, utils::ByteStream& pathDiffuseMap, utils::ByteStream& pathSpecularMap,
										   utils::ByteStream& pathLUT, UINT startSlotConstBuf = 0, UINT startSlotSRV = 0);

	private:
		void SetTexturesFromFiles(ID3D11Device* poDevice, const std::wstring& pathDiffuseMap,
								  const std::wstring& pathSpecularMap, const std::wstring& pathLUT = L"",
								  UINT startSlotSRV = 0);

		void SetTexturesFromMemory(ID3D11Device* poDevice, DxResourceManager& resourceManager, utils::ByteStream& pathDiffuseMap, utils::ByteStream& pathSpecularMap,
								   utils::ByteStream& pathLUT, UINT startSlotSRV = 0);
	};
}; // namespace rldx

#pragma once

#include "..\DataTypes\ConstBuffers\CPUConstBuffers.h"
#include "..\Interfaces\IBindable.h"
#include "DxConstBuffer.h"

#include <d3d11.h>
#include "..\..\ImportExport\Helpers\ByteStream.h"

#include <string>

namespace rldx {
	class DxTexture;	

	/// <summary>
	/// Sets 2/3 cubemaps needed for PBR imagebased lightng rendering
	/// </summary>	
	class DxAmbientLightSource : public IBindable
	{
		DxTexture* m_poDiffuseMap = nullptr;
		DxTexture* m_poSpecularMap = nullptr;
		DxTexture* m_poLUT = nullptr;

		UINT m_textureStartSlot = 0;	

	public:
		TDxPSShaderConstBuffer<PS_AmbientLight_Data_ConstBuffer> m_oPSConstBuffer;
		/// <summary>
		/// Bind IBL const buffer, and set cubemap textures
		/// </summary>
		/// <param name="poDC"></param>
		void BindToDC(ID3D11DeviceContext* poDC) override;

		static DxAmbientLightSource Create(
			ID3D11Device* poDevice,
			const std::wstring& pathDiffuseMap,
			const std::wstring& pathSpecularMap,
			const std::wstring& pathLUT = L"",
			UINT startSlotConstBuf = 0,
			UINT startSlotSRV = 0);

		static DxAmbientLightSource Create(
			ID3D11Device* poDevice,
			DxTexture* diffuseCubemap, 
			DxTexture* specularCubemap, 
			DxTexture* lut = nullptr,
			UINT startSlotConstBuf = 0,
			UINT startSlotSRV = 0);

		static DxAmbientLightSource Create(
			ID3D11Device* poDevice,
			ByteStream& pathDiffuseMap,
			ByteStream& pathSpecularMap,
			ByteStream& pathLUT,
			UINT startSlotConstBuf = 0,
			UINT startSlotSRV = 0);

		static DxAmbientLightSource Create(
			ID3D11Device* poDevice,
			DxTexture* diffuseCubemap,
			DxTexture* specularCubmap,
			DxTexture* lut,
			UINT startSlotConstBuf,
			UINT startSlotSRV);

	private:
		void SetTexturesFromFiles(
			ID3D11Device* poDevice,
			const std::wstring& pathDiffuseMap,
			const std::wstring& pathSpecularMap,
			const std::wstring& pathLUT = L"",
			UINT startSlotSRV = 0
		);

		void SetTexturesFromMemory(
			ID3D11Device* poDevice,
			ByteStream& pathDiffuseMap,
			ByteStream& pathSpecularMap,
			ByteStream& pathLUT, 
			UINT startSlotSRV = 0
		);

		

		
	};
}; // namespace rldx

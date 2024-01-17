#pragma once

#include <string>

#include "d3d11.h"

#include "..\Interfaces\IBindable.h"
#include "DxConstBuffer.h"
#include "..\DataTypes\ConstBuffers\CPUConstBuffers.h"

namespace Rldx {

	class DxTexture;
	
	class DxAmbientLight
	{
		DxTexture* m_poDiffuseMap;
		DxTexture* m_poSpecularMap;
		DxTexture* m_poLUT;

		UINT m_startSlotSRV = 0;

		TDxPSConstBuffer<PS_AmbientLight_ConstBuffer> m_oPSConstBuffer;

	public:
		int SetTextures(
			ID3D11Device* poDevice,
			const std::wstring& pathDiffuseMap,
			const std::wstring& pathSpecularMap,
			const std::wstring& pathLUT = L"",
			UINT startSlotSRV = 0
		);

		void BindToDC(ID3D11DeviceContext* poDC);
	};
}; // namespace Rldx

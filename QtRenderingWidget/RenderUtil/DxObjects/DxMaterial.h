#pragma once

#include <vector>
#include <d3d11.h>

#include "..\DirectXTK\Inc\CommonStates.h"

#include "DxTexture.h"
#include "..\Managers\DxResourceManager.h"

namespace Rldx {
			
	class DxMaterial
	{
		struct TextureAndSlot { UINT slot;  DxTexture* pTexture; };
		std::vector<TextureAndSlot> m_textures;

	public:
		void Init(ID3D11Device* device)
		{
			commonStates = DirectX::CommonStates(device);
		};

		void AddTexture(UINT slot, DxTexture* pTexture)
		{
			DxResourceManager::GetInstance().GetTextures().AddResource(*pTexture);
		}

		void Bind(ID3D11DeviceContext* poDC)
		{
			auto samplerLinearWrap = commonStates.LinearWrap();
			poDC->PSSetSamplers(0, 1, &samplerLinearWrap);

			for (auto& tex : m_textures)
			{
				tex.pTexture->BindAsShaderResourceView(poDC);
			}
		}

		DirectX::CommonStates commonStates;
	};

	class DxSampler

}


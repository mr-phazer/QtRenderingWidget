#pragma once

#include <vector>
#include <d3d11.h>

#include "..\DirectXTK\Inc\CommonStates.h"
#include "..\Managers\DxResourceManager.h"
#include "DxTexture.h"

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

		void AddTexture(UINT slot, const DxTexture* pTexture)
		{
			DxResourceManager::GetInstance()->GetTextures()->AddResource(pTexture);
		}

		void Bind(ID3D11DeviceContext* poDC);

		DirectX::CommonStates commonStates;
	};
};


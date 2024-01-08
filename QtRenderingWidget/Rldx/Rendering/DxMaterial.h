#pragma once

#include <vector>
#include <d3d11.h>

#include "..\DirectXTK\Inc\CommonStates.h"
#include "..\Managers\ResourceManager\DxResourceManager.h"
#include "..\Interfaces\IBindable.h"
#include "DxTexture.h"

namespace Rldx {

	class DxMaterial : public IDxResource, public IBindable
	{
		struct TextureAndSlot { UINT slot = 0;  DxTexture* pTexture = nullptr; } textureAndSlot;
		std::vector<TextureAndSlot> m_textures;

	public:
		void Init(ID3D11Device* device)
		{
		
		};

		void AddTexture(UINT slot, const DxTexture& texture)
		{
			// TODO: look at
			/*auto ret = DxResourceManager::GetInstance()->GetTextures()->AddResource(texture, "new texture");
			m_textures.push_back({ slot, ret.GetPtr()});*/
		}

		void BindToDC(ID3D11DeviceContext* poDC) override;

		// Inherited via IDxResource
		ResourceTypeEnum GetType() const override;	

		// Inherited via IDxResource
		std::string GetTypeString() const override;
	};
};


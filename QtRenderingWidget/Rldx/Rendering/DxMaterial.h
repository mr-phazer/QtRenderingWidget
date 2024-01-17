#pragma once

#include <vector>
#include <set>
#include <d3d11.h>

#include "..\DirectXTK\Inc\CommonStates.h"
#include "..\Managers\ResourceManager\DxResourceManager.h"
#include "..\Interfaces\IBindable.h"
#include "DxTexture.h"

namespace rldx {

	class DxMaterial : public IDxResource, public IBindable
	{
		struct RenderTextureElement
		{
			UINT slot = 0;
			DxTexture* pTexture = nullptr;
		};

		struct InputTextureElement
		{
			UINT type = 0;
			std::wstring path = L"";
		};

		std::vector<RenderTextureElement> m_textures;

		
		std::vector<std::string> m_pathHash;

	public:

		DxMaterial* Create(const std::vector<InputTextureElement>& textures =
			{
				{0, L"default_texture.dds"},
				{1, L"default_texture.dds"},
				{2, L"default_texture.dds"},
				{3, L"default_texture.dds"},
			}		
		)
		{
			return new DxMaterial();
		};


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
				
		ResourceTypeEnum GetType() const override;			
		std::string GetTypeString() const override;
	};
};


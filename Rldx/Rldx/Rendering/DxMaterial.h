#pragma once

#include <vector>
#include <set>
#include <d3d11.h>

#include "..\..\..\ImportExport\Helpers\ByteStream.h"
#include "..\..\..\ImportExport\RigidModel\DataTypes\RigidModelFile.h"

#include "..\..\..\DirectXTK\Inc\CommonStates.h"
#include "..\Managers\ResourceManager\DxResourceManager.h"
#include "..\Interfaces\IBindable.h"
#include "DxTexture.h"

namespace rldx {
	// for texture loading, maybe not needed
	struct InputTextureElement
	{
		UINT type = 0;
		std::wstring path = L"";
	};

	// Texture element for SRV
	struct RenderTextureElement
	{	
		DxTexture* pTexture = nullptr;
		std::wstring path = L"";
	};

	/// <summary>
	/// Material Class, contains texture with HLSL slot/"Texture Type"
	/// and and shader program
	/// </summary>
	class DxMaterial : public IDxResource, public IBindable
	{
		IDxShaderProgram* m_pShaderProgram = nullptr;

		std::map<TextureTypeEnum, RenderTextureElement> m_textures;
		std::vector<ID3D11ShaderResourceView*> m_emptyMaterial = std::vector<ID3D11ShaderResourceView*>(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, nullptr);

		std::string m_pathHash; // TODO: All the texture paths conceated to able to compared materials with operator==

	public:
		DxMaterial() = default;
		void InitWithDefaulTextures();
		bool operator==(const DxMaterial& other) const;

		std::string& GetPathHashRef() { return m_pathHash; };
		static DxMaterial* Create(ID3D11Device* poDevice, const std::vector<InputTextureElement>& textures =
			// TODO: "make sure, that the each 3 shaders have enough textureS to draw, no matter how many are missing, use deault textures"
			{
				{0, L"default_texture.dds"},				
			});

		void AddTexture(ID3D11Device* poDevice, UINT slot, const std::wstring& path);
		

		// Bind texture to DC, for doing a drawcall
		void BindToDC(ID3D11DeviceContext* poDC) override;
		void UnbindFromDC(ID3D11DeviceContext* poDC) override;
	
		bool IsValid() const  { return m_bIsValid; } ;
		int GetTextureStartSlot();

		ResourceTypeEnum GetType() const override;
		std::string GetTypeString() const override;

	private:
		DxTexture* LoadDefaultTexture(ID3D11Device* poDevice, UINT slot);
	private:
		bool m_bIsValid = true;
	};

	class IMaterialCreator
	{
	public:
		virtual DxMaterial* Create(ID3D11Device* poDevice) = 0;
	};

	class MaterialCreatorRMV2Mesh : public IMaterialCreator
	{
		const rmv2::MeshBlockCommon* m_data;
	public:
		MaterialCreatorRMV2Mesh(const rmv2::MeshBlockCommon& data) : m_data(&data) {};

		DxMaterial* Create(ID3D11Device* poDevice) override
		{
			auto newMaterial = DxResourceManager::Instance()->AllocMaterial().GetPtr();

			newMaterial->InitWithDefaulTextures();

			string hash = "";
			for (auto& tex : m_data->materialBlock.textureElements)
			{
				// TODO: clean up this "is material loaded right"-check
				auto diskPath = libtools::string_to_wstring(tex.texturePath);
				newMaterial->AddTexture(poDevice, tex.textureType, diskPath);
				newMaterial->GetPathHashRef() += std::string(tex.texturePath);
			};

			return newMaterial;
		}
	};

}
#pragma once

#include <d3d11.h>
#include <set>
#include <vector>

#include "..\..\..\ImportExport\FileFormats\RigidModel\Types\Common\RigidModelFile.h"
#include "Utils\ByteStream.h"

#include "..\..\..\DirectXTK\Inc\CommonStates.h"
#include "..\..\..\ImportExport\FileFormats\RigidModel\Types\Common\TextureElement.h"
#include "..\Interfaces\IBindable.h"
#include "..\Managers\ResourceManager\DxResourceManager.h"
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
		static std::vector<ID3D11ShaderResourceView*> m_emptyMaterial; // used for unbinding, made max size, as performance is not an issue

		std::string m_pathHash; // TODO: All the texture paths conceated to able to compared materials with operator==

	public:
		DxMaterial() = default;
		static DxMaterial* Create(std::vector<rmv2::TextureElement>& textures);

		void InitWithDefaulTextures();
		bool operator==(const DxMaterial& other) const;
		void SetTextures(ID3D11Device* poDevice, const std::vector<rmv2::TextureElement>& inTex);

		std::string& PathHash() { return m_pathHash; };
		static DxMaterial* Create(ID3D11Device* poDevice, const std::vector<InputTextureElement>& textures =
								  // TODO: "make sure, that the each 3 shaders have enough textureS to draw, no matter how many are missing, use deault textures"
								  {
									  {0, L"default_texture.dds"},
								  });

		void AddTexture(ID3D11Device* poDevice, UINT slot, const std::wstring& path);


		// Bind texture to DC, for doing a drawcall
		void BindToDC(ID3D11DeviceContext* poDC) override;
		void UnbindFromDC(ID3D11DeviceContext* poDC) override;

		bool IsValid() const { return m_bIsValid; };
		int GetTextureStartSlot();

	private:
		DxTexture* LoadDefaultTexture(ID3D11Device* poDevice, UINT slot);
	private:
		bool m_bIsValid = true;

		std::map<TextureTypeEnum, std::wstring> m_defaultTexturesMap = {
			{TextureTypeEnum::eBaseColor, L"default_base_colour.dds"},
			{TextureTypeEnum::eDiffuse, L"default_grey.dds"},
			{TextureTypeEnum::eSpecular, L"default_specular.dds"},
			{TextureTypeEnum::eGlossMap, L"default_gloss_map.dds"},
			{TextureTypeEnum::eMaterialMap, L"default_material_map.dds"},
			{TextureTypeEnum::eNormal, L"default_normal.dds"},
		};

	};

	class IMaterialCreator
	{
	public:
		virtual DxMaterial* Create(ID3D11Device* poDevice) = 0;
	};

	class MaterialCreatorRMV2Mesh : public IMaterialCreator
	{
		const rmv2::MeshBlockCommon* data;
	public:
		MaterialCreatorRMV2Mesh(const rmv2::MeshBlockCommon& data) : data(&data) {};

		DxMaterial* Create(ID3D11Device* poDevice) override
		{
			auto newMaterial = DxResourceManager::Instance()->AllocMaterial().GetPtr();

			newMaterial->InitWithDefaulTextures();

			std::string hash = "";
			for (auto& tex : data->materialBlock.textureElements)
			{
				// TODO: clean up this "is material loaded right"-check
				auto diskPath = utils::ToWString(tex.texturePath);
				newMaterial->AddTexture(poDevice, tex.textureType, diskPath);
				newMaterial->PathHash() += std::string(tex.texturePath);
			};

			return newMaterial;
		}
	};

}

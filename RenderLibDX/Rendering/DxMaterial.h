#pragma once

#include <vector>
#include <set>
#include <d3d11.h>

#include "..\..\ImportExport\Helpers\ByteStream.h"
#include "..\..\ImportExport\RigidModel\DataTypes\RigidModelFile.h"

#include "..\DirectXTK\Inc\CommonStates.h"
#include "..\Managers\ResourceManager\DxResourceManager.h"
#include "..\Interfaces\IBindable.h"
#include "DxTexture.h"

static std::wstring defaultTexturePath = LR"(K:\Modding\WH2\)";

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
		UINT slot = 0; // type(abledo/normal) used as slot in shader
		DxTexture* pTexture = nullptr;
	};

	/// <summary>
	/// Material Class, contains texture with HLSL slot/"Texture Type"
	/// and and shader program
	/// </summary>
	class DxMaterial : public IDxResource, public IBindable
	{
		IDxShaderProgram* m_pShaderProgram = nullptr;

		std::vector<RenderTextureElement> m_textures;

		std::vector<std::string> m_pathHash; // TODO: All the texture patha conceated to compare == materials

	public:
		DxMaterial() = default;
		static DxMaterial* Create(ID3D11Device* poDevice, const std::vector<InputTextureElement>& textures =
			{
				{0, L"default_texture.dds"},
				{1, L"default_texture.dds"},
				{2, L"default_texture.dds"},
				{3, L"default_texture.dds"},
			}
			)
		{
			auto newMaterial = DxResourceManager::Instance()->AllocMaterial().GetPtr();

			for (auto& tex : textures)
			{
				newMaterial->AddTexture(poDevice, tex.type, tex.path);
			};

			return new DxMaterial();
		};

		void AddTexture(ID3D11Device* poDevice, UINT slot, const std::wstring& path)
		{
			auto textPtr = DxResourceManager::Instance()->AllocTexture().GetPtr();

			textPtr->LoadFile(poDevice, path);

			m_textures.push_back({ slot, textPtr });
		}

		void BindToDC(ID3D11DeviceContext* poDC) override;

		ResourceTypeEnum GetType() const override;
		std::string GetTypeString() const override;
	};



	class IMaterialCreator
	{
	public:
		virtual DxMaterial* Create(ID3D11Device* poDevice) = 0;
	};

	class MaterialCreatorRMV2Mesh : public IMaterialCreator
	{
		rmv2::MeshBlockCommon* m_data;
	public:
		MaterialCreatorRMV2Mesh(rmv2::MeshBlockCommon& data) : m_data(&data) {};

		DxMaterial* Create(ID3D11Device* poDevice) override
		{
			auto newMaterial = DxResourceManager::Instance()->AllocMaterial().GetPtr();

			string hash = "";
			for (auto& tex : m_data->materialBlock.textureElements)
			{
				auto diskPath = defaultTexturePath + tools::string_to_wstring(tex.szTexturePath);
				newMaterial->AddTexture(poDevice, tex.dwTextureType, diskPath);
				hash += std::string(tex.szTexturePath);
			};

			return newMaterial;			
		}

	};

}
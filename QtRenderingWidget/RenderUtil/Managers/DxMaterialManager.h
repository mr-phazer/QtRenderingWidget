#pragma once
#include <vector>
#include <string>
#include <unordered_map>



namespace rendutil {

	class DxTextureView;

	class Texture
	{
		bool ComparePaths(const Texture&);

		uint32_t shaderIndex; // where the tex buffer is bound in the shader
		DxTextureView* texView;
		std::string path;
	};

	class DxMaterial
	{
		/// <summary>
		/// To see if identical material already exists in the scene
		/// </summary>		
		bool ComparePaths(const DxMaterial&);
		std::vector<Texture> textures;
	};

	class DxMaterialManager
	{
		DxMaterial* GetMaterial(uint32_t index);
		void SetMaterial(DxMaterial* material);

	private:
		std::unordered_map<uint32_t, DxMaterial*> m_materials;
	};

}; // namespace rendutil
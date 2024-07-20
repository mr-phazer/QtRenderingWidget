#pragma once

#include <vector>
#include "..\..\..\FileFormats\XmMaterial\DataTypes\XMLMaterialData.h"
#include <CommonLibs/Logger/Logger.h>

namespace rmv2 {

	struct WsModelData
	{
		bool IsValid()
		{
			if (xmlMateriData.empty()) // whole 2d array empty ?
				return false;

			if (xmlMateriData[0].empty()) // lod 0 empty?
				return false;

			return true;
		}

		void AddXmlMaterial(XMLMaterialData& input, size_t lodIndex, size_t partIndex)
		{
			// resize 2d vectors as needed
			if (lodIndex >= xmlMateriData.size()) {
				xmlMateriData.resize(lodIndex + 1);
			}

			if (partIndex >= xmlMateriData[lodIndex].size()) {
				xmlMateriData[lodIndex].resize(partIndex + 1);
			}

			xmlMateriData[lodIndex][partIndex] = input;
		}

		std::vector<TextureElement> GetMeshMaterial(size_t lod, size_t mesh)
		{
			if (lod >= xmlMateriData.size()) {
				throw std::exception(std::string(FULL_FUNC_INFO("Lod index out of bound")).c_str());
			}

			if (mesh >= xmlMateriData[lod].size()) {
				throw std::exception(std::string(FULL_FUNC_INFO("mesh index out of bound")).c_str());
			}

			return xmlMateriData[lod][mesh].textures;
		}


		std::wstring geometryPath;
		std::vector<std::vector<XMLMaterialData>> xmlMateriData;
	};
}
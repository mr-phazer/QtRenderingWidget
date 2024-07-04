#pragma once

#include "..\..\..\..\..\ImportExport\Helpers\Templates.h"
#include "..\..\..\..\..\QtRenderingWidget\Constants\GameIdKeys.h"
#include "..\..\..\SceneGraph\SceneNodes\DxVmdNodes.h"
//#include "VmdNodeCreators.h"


// TODO: move this class to ImportExport
namespace rldx
{
	class IDxMaterialInfoReader
	{
	public:

		virtual void Parse() = 0;


	};
	/// <summary>
	/// Find material info by pase thse files
	/// </summary>
	class DxMaterialInfoReader : public IDxMaterialInfoReader
	{
		VMDNodeData* m_pVmdModeData;
	public:
		DxMaterialInfoReader(VMDNodeData* pVmdModeData) : m_pVmdModeData(pVmdModeData) {};

		void Parse()
		{
			if (CompareExtension(m_pVmdModeData->varintMeshData.modelPath, FileExtensions::RigidModelV2))
			{
				GetMaterialsFromRmv2();
			}
			else if (CompareExtension(m_pVmdModeData->varintMeshData.modelPath, FileExtensions::WSmodel))
			{
				GetMaterialsWSModel();
			}
		}

	private:
		void GetMaterialsFromRmv2()
		{
			if (m_pVmdModeData->varintMeshData.modelPath.empty()) {
				return;
			}

			auto bytesRmv2 = DxResourceManager::GetFile(m_pVmdModeData->varintMeshData.modelPath);
			auto Rmv2File = rmv2::RigidModelReader().Read(bytesRmv2);

			if (bytesRmv2.GetBufferSize() == 0)
			{
				m_pVmdModeData->varintMeshData.wsModelData.geometryPath = L"";
				return;
			}

			// set geometry path (= where the rmv2 file is) from the WSMODEL data, so RMV2/WSMODEL reads their geomtry path fromm the same place
			m_pVmdModeData->varintMeshData.wsModelData.geometryPath = m_pVmdModeData->varintMeshData.modelPath;

			bool bUsesSpecGloss = false;
			for (size_t iLod = 0; iLod < Rmv2File.lods.size(); iLod++)
			{
				for (size_t iMesh = 0; iMesh < Rmv2File.lods[iLod].meshBlocks.size(); iMesh++)
				{
					rmv2::XMLMaterialData meshMaterial;
					meshMaterial.textures = Rmv2File.lods[iLod].meshBlocks[iMesh].materialBlock.textureElements; // copy RMV2 texture info into the material structure

					if (DxResourceManager::Instance()->GetGameIdString() == game_id_keys::KEY_WARHAMMER_3)
					{
						CorrectWH3TexturePaths(meshMaterial);
					}

					m_pVmdModeData->varintMeshData.wsModelData.AddXmlMaterial(meshMaterial, iLod, iMesh); // set the material structure as the materials for this node
				}


			}

		}

		void CorrectWH3TexturePaths(rmv2::XMLMaterialData& meshMaterial)
		{
			for (auto& itTex : meshMaterial.textures)
			{
				if (itTex.textureType == TextureTypeEnum::eDiffuse)
				{
					std::string pathRoot = itTex.texturePath;
					pathRoot.erase(pathRoot.end() - std::string("diffuse.dds").length(), pathRoot.end());

					meshMaterial.textures.clear();

					meshMaterial.AddTexture(TextureTypeEnum::eBaseColor, pathRoot + "base_colour.dds");
					meshMaterial.AddTexture(TextureTypeEnum::eMaterialMap, pathRoot + "material_map.dds");
					meshMaterial.AddTexture(TextureTypeEnum::eNormal, pathRoot + "normal.dds");
					meshMaterial.AddTexture(TextureTypeEnum::eMask, pathRoot + "mask.dds");

					break;
				}
			}
		}

		void GetMaterialsWSModel()
		{
			auto bytesWsModel = DxResourceManager::GetFile(m_pVmdModeData->varintMeshData.modelPath);

			if (bytesWsModel.GetBufferSize() == 0)
			{
				m_pVmdModeData->varintMeshData.wsModelData.geometryPath = L"";
				return;
			}

			rmv2::WsModelReader wsModelReader;
			m_pVmdModeData->varintMeshData.wsModelData = wsModelReader.Read(bytesWsModel);
		}
	};
}
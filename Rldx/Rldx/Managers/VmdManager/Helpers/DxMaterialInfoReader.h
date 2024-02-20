#pragma once

#include "..\..\..\..\..\ImportExport\Helpers\Templates.h"
#include "..\..\Rldx\Rldx\Tools\tools.h"
#include "..\..\..\SceneGraph\SceneNodes\DxVmdNode.h"
#include "VmdNodeCreators.h"


namespace rldx
{
	/// <summary>
	/// Find material info by pase thse files
	/// </summary>
	class DxMaterialInfoReader
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

			auto bytesRmv2 = DxResourceManager::GetCallBackFile(m_pVmdModeData->varintMeshData.modelPath);
			auto Rmv2File = rmv2::RigidModelReader().Read(bytesRmv2);

			// set geometry path is WSMODEL data, so RMV2/WSMODEL reads their geomtry path fromm the same place
			m_pVmdModeData->varintMeshData.wsModelData.geometryPath = m_pVmdModeData->varintMeshData.modelPath;

			for (size_t iLod = 0; iLod < Rmv2File.lods.size(); iLod++)
			{
				for (size_t iMesh = 0; iMesh < Rmv2File.lods[iMesh].meshBlocks.size(); iMesh++)
				{
					rmv2::XMLMaterialData meshMaterial;
					meshMaterial.textures = Rmv2File.lods[iLod].meshBlocks[iMesh].materialBlock.textureElements;

					m_pVmdModeData->varintMeshData.wsModelData.AddXmlMaterial(meshMaterial, iLod, iMesh);
				}
			}
		}

		void GetMaterialsWSModel()
		{
			auto bytesWsModel = DxResourceManager::GetCallBackFile(m_pVmdModeData->varintMeshData.modelPath);

			rmv2::WsModelReader wsModelReader;
			m_pVmdModeData->varintMeshData.wsModelData = wsModelReader.Read(bytesWsModel);			
		}
	};
}
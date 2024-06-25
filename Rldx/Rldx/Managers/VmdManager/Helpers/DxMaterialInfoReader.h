#pragma once

#include "..\..\..\..\..\ImportExport\Helpers\Templates.h"
#include "..\..\..\SceneGraph\SceneNodes\DxVmdNodes.h"
#include "..\..\Rldx\Rldx\Tools\tools.h"
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

			// set geometry path is WSMODEL data, so RMV2/WSMODEL reads their geomtry path fromm the same place
			m_pVmdModeData->varintMeshData.wsModelData.geometryPath = m_pVmdModeData->varintMeshData.modelPath;

			for (size_t iLod = 0; iLod < Rmv2File.lods.size(); iLod++)
			{
				for (size_t iMesh = 0; iMesh < Rmv2File.lods[iLod].meshBlocks.size(); iMesh++)
				{
					rmv2::XMLMaterialData meshMaterial;
					meshMaterial.textures = Rmv2File.lods[iLod].meshBlocks[iMesh].materialBlock.textureElements;

					m_pVmdModeData->varintMeshData.wsModelData.AddXmlMaterial(meshMaterial, iLod, iMesh);
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
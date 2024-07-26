#include "DxMeshRenderData.h"

#include "..\Managers\DXdeviceManager.h"
#include "..\Managers\ResourceManager\DxResourceManager.h"
#include "..\Rendering\DxShaderProgram.h"
#include "..\SceneGraph\SceneNodes\DxDeformerNode.h"
#include "DxMesh.h"

namespace rldx {
	DxMeshRenderingData::DxMeshRenderingData()
	{
		CreateConstBuffers(DxDeviceManager::Device());
	}

	DxMeshRenderingData::~DxMeshRenderingData()
	{
		// TODO: manual deallocate should not be needed
		/*logging::LogAction(L"Delaolocation of: Material, Mesh, shader, from DxMeshRenderingData: " + meshName);
		DxResourceManager::FreeMemoryFromPtr(poMaterial);
		DxResourceManager::FreeMemoryFromPtr(poMesh);
		DxResourceManager::FreeMemoryFromPtr(poShaderProgram);*/
	}

	void DxMeshRenderingData::CreateConstBuffers(ID3D11Device* poDevice)
	{
		perMesh_VS_CB.Init(poDevice, "VS_PerMesh_CB");
		perMeshDerformer_VS_CB.Init(poDevice, "VS_PerMesh_Derformer");

		perMesh_PS_CB.Init(poDevice, "PS_PerMesh");
	}

	void DxMeshRenderingData::Update(float time)
	{
	}

	void DxMeshRenderingData::Draw(ID3D11DeviceContext* pDeviceContext)
	{
		poMesh->Draw(pDeviceContext);
	}

	void DxMeshRenderingData::BindToDC(ID3D11DeviceContext* pDeviceContext)
	{
		if (poMesh) poMesh->BindToDC(pDeviceContext);
		if (poShaderProgram) poShaderProgram->BindToDC(pDeviceContext);
		else {
			auto DEBUG__BREAK_1 = 1;
		}
		if (poMaterial) poMaterial->BindToDC(pDeviceContext);


		if (perMesh_VS_CB.GetBuffer())
		{
			perMesh_VS_CB.SetStartSlot(1);
			perMesh_VS_CB.BindToDC(pDeviceContext);
		}

		if (perMesh_PS_CB.GetBuffer())
		{
			perMesh_PS_CB.SetStartSlot(2); // slot 0 = ambient light, 1 = directional light const buffers
			perMesh_PS_CB.BindToDC(pDeviceContext);
		}

		// TODO: this mesh should not get get the data directly this like
		/*
			rather, renderquque should be sorted

			skeleton 1
			mesh, deformer = 1
			mesh, deformer = 1

			skeleton 2
			mesh, deformer = 2
			mesh, deformer = 2


		*/

		// TODO: MOVE TO "Update()"!!!
		perMeshDerformer_VS_CB.SetStartSlot(2);

		// TODO: Change!! This causes MASSIVE lag
		if (poDeformerSourceNode)
		{
			auto skeletonVSConstBufferData = poDeformerSourceNode->GetDeformerData();
			perMeshDerformer_VS_CB.data = *skeletonVSConstBufferData;

			if (attachPoint.boneIndex != -1)
			{
				auto correctMatrix = sm::Matrix::CreateFromYawPitchRoll(DirectX::XM_PI, -DirectX::XM_PIDIV2, 0);
				perMesh_VS_CB.data.mWeaponWorld = skeletonVSConstBufferData->boneTransform[attachPoint.boneIndex] * correctMatrix; //  TODO: MAYBE bounds checking??
				perMesh_VS_CB.data.pivot = pivot;
			}
		}

		perMesh_VS_CB.BindToDC(pDeviceContext);
		perMeshDerformer_VS_CB.BindToDC(pDeviceContext);

		// TODO: do this to anime static attached meshes
			/*
			ALGO:
			if attachpoint != -1
				vscbPerMesh.mWorld = poDeformerSourceNode->GetDeformerData()[attachPoint.boneIndex] * this->mWorld;


			*/

	}
} // namespace rldx

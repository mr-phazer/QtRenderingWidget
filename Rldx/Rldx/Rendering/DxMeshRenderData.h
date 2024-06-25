#pragma once

#include <memory>
#include "..\DataTypes\ConstBuffers\CPUConstBuffers.h"
#include "..\Interfaces\IRenderQueueItem.h"
#include "DxConstBuffer.h"

namespace rldx
{
	class DxMeshNode;
	class DxBaseNode;
	class DxMesh;
	class DxMaterial;
	class DxMeshShaderProgram;
	class DxDeformerNode;

	struct DxMeshRenderingData : public IRenderQueueItem
	{
		DxMeshRenderingData();

		std::wstring meshName;
		DxMesh* poMesh = nullptr;
		DxMaterial* poMaterial = nullptr;
		DxMeshShaderProgram* poShaderProgram = nullptr;
		const DxDeformerNode* poDeformerSourceNode = nullptr;
		sm::Vector3 pivot;

		struct AttachPoint
		{
			int32_t boneIndex = -1;
			sm::Matrix mWorldPreMultiply;
		}
		attachPoint;

		float visibilityDistance = 0.0f;

		TDxVSShaderConstBuffer<VS_PerMeshConstBuffer_Skeleton> perMeshDerformer_VS_CB;
		TDxVSShaderConstBuffer<VS_PerMesh_ConstBuffer> perMesh_VS_CB;
		TDxPSShaderConstBuffer<PS_PerMesh_ConstBuffer> perMesh_PS_CB;



		virtual void Update(float time) override; // TODO: update deforming stuff
		virtual void Draw(ID3D11DeviceContext* pDeviceContext) override;
		virtual void BindToDC(ID3D11DeviceContext* pDeviceContext) override;

		void CreateConstBuffers_DOES_NOTHING__REMOVE(ID3D11Device* poDevice);

	private:
		void CreateConstBuffers(ID3D11Device* poDevice);

	};
} // namespace rldx

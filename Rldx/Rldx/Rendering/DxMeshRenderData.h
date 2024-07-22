#pragma once

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

	enum class MeshTypeEnum { DefaultMesh, BBMesh, SkeleteonMesh };

	struct DxMeshRenderingData : public IRenderQueueItem
	{
		DxMeshRenderingData();
		virtual ~DxMeshRenderingData();

		MeshTypeEnum m_meshType = MeshTypeEnum::DefaultMesh;

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

		DirectX::BoundingBox boundingBox;

		virtual void Update(float time) override; // TODO: update deforming stuff
		virtual void Draw(ID3D11DeviceContext* pDeviceContext) override;
		virtual void BindToDC(ID3D11DeviceContext* pDeviceContext) override;

	private:
		void CreateConstBuffers(ID3D11Device* poDevice);

	};
} // namespace rldx

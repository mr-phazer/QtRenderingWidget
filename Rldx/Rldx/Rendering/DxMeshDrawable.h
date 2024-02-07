#pragma once

#include "DxConstBuffer.h"
#include "..\DataTypes\DerformerData.h"
#include "..\Interfaces\IRenderQueueItem.h"

namespace rldx {

	class DxMeshNode;
	class DxMesh;	
	class DxMaterial;
	class DxMeshShaderProgram;

	typedef TDxVSShaderConstBuffer<DxDeformerData> DxDeformerDataConstBuffer;

	class DxMeshDrawable : public IRenderQueueItem
	{
		// TODO: use friend or interface methods
		friend class DxMeshNode;

		DxMesh* m_poMesh = nullptr;
		DxMaterial* m_poMaterial = nullptr;
		DxMeshShaderProgram* m_poShaderProgram = nullptr;
		DxDeformerData m_deformerDataBuffer; // only used if node is a a skeleton/defomer node
		DxMeshNode* m_pDeformerNode = nullptr; // points to deformer/skeleton node, that will deform this mesh
		
		// TODO: use this to move attached meshes (weapons/shields in VMDs)
		sm::Matrix m_mWorldMatrix;
		sm::Vector3 m_pivotPoint;
		uint32_t boneIndex = 0; // TODO: is this needed

		// TODO: add DxBoundingBox		
				
		void SetMeshData(DxMesh* meshData) { m_poMesh = meshData; };
		void SetMaterial(DxMaterial* material) { m_poMaterial = material; };
		void SetShaderProgram(DxMeshShaderProgram* shaderProgram) { m_poShaderProgram = shaderProgram; };
		void SetDeformerNode(DxMeshNode* pDeformerNode) { m_pDeformerNode = pDeformerNode; };

		virtual void Draw(ID3D11DeviceContext* pDeviceContext) override;
		virtual void BindToDC(ID3D11DeviceContext* pDeviceContext) override;
	};

};


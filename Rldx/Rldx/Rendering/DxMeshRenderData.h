#pragma once

#include "..\DataTypes\DerformerData.h"
#include "..\Interfaces\IRenderQueueItem.h"
#include "DxConstBuffer.h"

namespace rldx {

	class DxMeshNode;
	class DxBaseNode;
	class DxMesh;
	class DxMaterial;
	class DxMeshShaderProgram;
	class DxDeformerNode;

	typedef TDxVSShaderConstBuffer<DxDeformerData> DxDeformerDataConstBuffer;

	struct DxMeshRenderData : public IRenderQueueItem
	{
		DxMesh* m_poMesh = nullptr;
		DxMaterial* m_poMaterial = nullptr;
		DxMeshShaderProgram* m_poShaderProgram = nullptr;
		DxDeformerData m_deformerDataBuffer; // only used if node is a a skeleton/defomer node		

		// TODO: use this to move attached meshes (weapons/shields in VMDs)
		sm::Matrix m_mWorldMatrix = sm::Matrix::Identity;;
		sm::Matrix m_mModelSpaceMatrix = sm::Matrix::Identity;
		sm::Vector3 m_pivotPoint = { 0,0,0 };

		DxDeformerNode* m_pDeformerNode = nullptr;
		int attachIndex = -1; // the skeleton bone that will move THIS node


		virtual void Update(float time) override; // TODO: update deforming stuff
		virtual void Draw(ID3D11DeviceContext* pDeviceContext) override;
		virtual void BindToDC(ID3D11DeviceContext* pDeviceContext) override;
	};

};


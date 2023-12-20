#pragma once

#include "DxSceneNode.h"
#include "..\..\DxObjects\DxMesh.h"
#include "..\..\..\Logging\Logging.h"

namespace Rldx
{
	class DxMeshNode : public DxSceneNode, IDrawable
	{

		DxMesh* m_meshData = nullptr;
		DxMaterial* m_material = nullptr;
		DxMeshShaderProgam* m_shaderProgram = nullptr;

	public:
		using Sptr = std::shared_ptr<DxMeshNode>;
	public:
		DxMeshNode(const std::string& name) { SetName(name); };

		static DxMeshNode::Sptr Create(const std::string& name = "");

		void SetShaderProgram(DxMeshShaderProgam* shaderProgram) { m_shaderProgram = shaderProgram; };

		void SetMeshData(const DxCommonMeshData& meshData)
		{
			auto newMeshHandle = DxResourceManager::Instance()->AllocMesh();
			m_meshData = newMeshHandle.GetPtr();
			m_meshData->SetMeshData(meshData);

			// TODO: REMOVE:
			auto DEBUG_BREAK = 1;
		};

		void Draw(ID3D11DeviceContext* poDC) override
		{

			// ready shader program
			m_shaderProgram->GetReady(poDC);

			// draw mesh
			m_meshData->Draw(poDC);

		};
		
		std::string GetTypeString() const override { return "DxMeshNode"; };

	public:
		//void Draw(ID3D11DeviceContext* poDC, IDxShaderProgram* shaderProgram = nullptr, ID3D11RenderTargetView* destRtV = nullptr) override
		//{
		//	// TODO: finish, draw mesh

		//	// if RTV is supplied set that as active
		//	if (destRtV != nullptr) {
		//		poDC->OMSetRenderTargets(1, &destRtV, nullptr);
		//	}

		//	GetMeshReadyForDrawing(poDC);

		//	poDC->DrawIndexed(m_meshData->indexCount, 0, 0);
		//};


	/*	virtual void GetMeshReadyForDrawing(ID3D11DeviceContext* poDC)
		{
			UINT stride = sizeof(CommonVertex);
			UINT offset = 0;

			poDC->IASetVertexBuffers(0, 1, m_meshData->m_cpoVertexBuffer.GetAddressOf(), &stride, &offset);
			poDC->IASetIndexBuffer(m_meshData->m_cpoIndexBuffer.Get(), m_meshData->m_enumIndexFormat, 0);

			poDC->IASetPrimitiveTopology(m_meshData->m_enumTopology);
		};*/



	};
};
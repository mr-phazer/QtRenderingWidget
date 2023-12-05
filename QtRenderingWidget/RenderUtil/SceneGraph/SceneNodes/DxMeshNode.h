#pragma once

#include "DxSceneNode.h"
#include "..\..\DxObjects\DxMesh.h"

namespace Rldx
{
	class DxMeshNode : public DxSceneNode, IDrawable
	{
		DxMesh* m_mesh;
		DxMaterial* m_material;
		DxMeshShaderProgam* m_shaderProgram;

	public:
		using Sptr = std::shared_ptr<DxMeshNode>;
	public:
		DxMeshNode() = default;
		virtual ~DxMeshNode() = default;
		
		static DxMeshNode::Sptr Create(std::string name = "");
				
		void Draw(ID3D11DeviceContext* poDC, ID3D11RenderTargetView* destRtV = nullptr) override
		{
			// TODO: finish, draw mesh			

			m_shaderProgram->GetReady(poDC);

			m_material->

			m_mesh->Draw(poDC, nullptr, destRtV);

			
		};

	public:
		void Draw(ID3D11DeviceContext* poDC, IDxShaderProgram* shaderProgram = nullptr, ID3D11RenderTargetView* destRtV = nullptr) override
		{
			// TODO: finish, draw mesh

			// if RTV is supplied set that as active
			if (destRtV != nullptr) {
				poDC->OMSetRenderTargets(1, &destRtV, nullptr);
			}

			GetMeshReadyForDrawing(poDC);

			poDC->DrawIndexed(m_meshData->indexCount, 0, 0);
		};

		
		virtual void GetMeshReadyForDrawing(ID3D11DeviceContext* poDC)
		{
			UINT stride = sizeof(CommonVertex);
			UINT offset = 0;

			poDC->IASetVertexBuffers(0, 1, m_meshData->m_cpoVertexBuffer.GetAddressOf(), &stride, &offset);
			poDC->IASetIndexBuffer(m_meshData->m_cpoIndexBuffer.Get(), m_meshData->m_enumIndexFormat, 0);

			poDC->IASetPrimitiveTopology(m_meshData->m_enumTopology);
		};



	};
};
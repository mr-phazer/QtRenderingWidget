#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <typeinfo>
#include <cassert>

#include "..\..\..\DirectXTK\Inc\GeometricPrimitive.h"
#include "..\..\..\DirectXTK\Inc\VertexTypes.h"
#include "..\Managers\ResourceManager\DxResourceManager.h"

#include "..\..\..\Interfaces\IDrawable.h"
#include "DxShaderProgram.h"
#include "DxMaterial.h"
#include "..\Types\DxMeshData.h"

namespace Rldx {

	class DxMesh : public IDrawable, public IDxResource
	{
	public:
		void Draw(ID3D11DeviceContext* poDC) override
		{	
			GetMeshReadyForDrawing(poDC);

			poDC->DrawIndexed(m_meshData.indexCount, 0, 0);
		};

		virtual void GetMeshReadyForDrawing(ID3D11DeviceContext* poDC)
		{		
			UINT stride = sizeof(CommonVertex); 
			UINT offset = 0;
			poDC->IASetVertexBuffers(0, 1, m_meshData.cpoVertexBuffer.GetAddressOf(), &stride, &offset);
			poDC->IASetIndexBuffer(m_meshData.cpoIndexBuffer.Get(), m_meshData.enumIndexFormat, 0);
			poDC->IASetPrimitiveTopology(m_meshData.enumTopology);			
		};

		// Inherited via IDxResource
		ResourceTypeEnum GetType() const override;


		void SetMeshData(const DxCommonMeshData& meshData);;

	private:
		DxCommonMeshData m_meshData;

		// Inherited via IDxResource
		std::string GetTypeString() const override;
	};

	// TODO: use or delete?
	//class MeshRenderer
	//{
	//public:
	//	static void Draw(
	//		ID3D11DeviceContext* poDC,
	//		DxMesh* poMesh,
	//		IDxShaderProgram* poShaderProgram,
	//		DxMaterial* poMaterial,
	//		ID3D11RenderTargetView* destRtV = nullptr
	//	)
	//	{
	//		// if RTV is supplied set that as active
	//		if (destRtV != nullptr) {
	//			poDC->OMSetRenderTargets(1, &destRtV, nullptr);
	//		}

	//		GetMeshReadyForDrawing(poDC);

	//		poDC->DrawIndexed(m_meshData.indexCount, 0, 0);


	//	}

	//private:
	//	static void GetMaterialReady(ID3D11DeviceContext* poDC, DxMaterial* material);
	//	static void GetShadersReady(ID3D11DeviceContext* poDC, IDxShaderProgram* shaderProgram);
	//	static void GetMeshReadyForDrawing(ID3D11DeviceContext* poDC)
	//	{

	//	}
	//};
}; // namespace Rldx


#pragma once

#include <memory>
#include <windows.h>

#include "DXSwapChain.h"
#include "IDrawable.h"
#include "..\SceneGraph\SceneNodes\DxMeshNode.h"
#include "..\Types\ConstBuffers\CPUConstBuffers.h"

namespace Rldx {

	
	class DxScene : public IDrawable
	{
		friend class NativeWindowSceneCreator;
	
	public:	
		using UniquePtr = std::unique_ptr<DxScene>;

	public:
		virtual void Draw(
			ID3D11DeviceContext* poDeviceContext,
			ID3D11RenderTargetView* destRtV = nullptr
			) override
		{
			// clear back buffer
			m_spoSwapChain->GetBackBuffer()->ClearPixels(poDeviceContext, { 0, 0.5f, 0, 1 });
						
			// set back buffer to active render target
			m_spoSwapChain->GetBackBuffer()->BindAsRenderTargetView(poDeviceContext);

			// update + set scene constant buffer
			m_vertexShaderConstantBuffer.SetData(poDeviceContext, m_vertexShaderconstantBufferData);
			ID3D11Buffer* vertexShaderSceneConstBuffers[1] = { m_vertexShaderConstantBuffer.GetBuffer() };
			poDeviceContext->VSSetConstantBuffers(0, 1, vertexShaderSceneConstBuffers);
						
			DxSceneNode* poChild = GetRootNode()->GetChild();			
			DxMeshNode* poMeshNode = dynamic_cast<DxMeshNode*>(poChild);
			if (poMeshNode)
			{
				poMeshNode->Draw(poDeviceContext);
			}


			/*
				- first test, draw one simple mesh.


				- process scene graph to get all meshnodes
				- put mesh nodes in a container "class DxRenderQueue"
				- for each mesh node in DxRenderQueue
				  - user DxRenderPass::Draw(DxRenderQueue queueToDraw)




			*/

			m_spoSwapChain->Present(poDeviceContext);
		};

		virtual void Init(ID3D11Device* poDevice, HWND nativeWindowHandle);

		DxSceneNode* GetRootNode();
		void DeleteNode(DxSceneNode* node);

		DxSwapChain* GetSwapChain()
		{
			return m_spoSwapChain.get();
		}

		DxSwapChain::UPtr& GetRefSwapChain()
		{
			return m_spoSwapChain;
		}

	private:
		DxSwapChain::UPtr m_spoSwapChain; // back buffer
		DxSceneNode::Sptr m_spoRootNode = DxSceneNode::Create("RootNode");

		// TODO: iterate over these, call IResizable::rest(width, height)
		std::vector<IResizable*> m_resizableObjects; // buffers/etc, that need to be resized when the window is resized
		
		// TODO: should this be 2 member? think about integrated solution
		VS_SceneConstantBuffer m_vertexShaderconstantBufferData; // constant buffer data for the vertex shader
		DirectX::ConstantBuffer<VS_SceneConstantBuffer> m_vertexShaderConstantBuffer; // constant buffer for the vertex shader
	};


	class IDxSceneCreator
	{
	public:
		virtual DxScene::UniquePtr Create(ID3D11Device* poDevice) = 0;
		

	};

	class NativeWindowSceneCreator : IDxSceneCreator
	{
		NativeWindowSceneCreator() {};	

	public:
		NativeWindowSceneCreator(HWND nativeWindowHandle) : m_nativeWindowHandle(nativeWindowHandle) {};
		DxScene::UniquePtr Create(ID3D11Device* poDevice)
		{
			auto newScene = std::make_unique<DxScene>();
			newScene->GetRefSwapChain() = DxSwapChain::CreateForHWND(poDevice, m_nativeWindowHandle);
			CreateViewAndPerspective(*newScene);

			return std::move(newScene);
		};
	private:
		void CreateViewAndPerspective(DxScene& scene)
		{
			// Use DirectXMath to create view and perspective matrices.

			DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.7f, 1.5f, 0.f);
			DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, -0.1f, 0.0f, 0.f);
			DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.f);

			DirectX::XMStoreFloat4x4(
				&scene.m_vertexShaderconstantBufferData.view,
				DirectX::XMMatrixTranspose(
					DirectX::XMMatrixLookAtRH(
						eye,
						at,
						up
					)
				)
			);

			float aspectRatioX = scene.GetSwapChain()->GetBackBuffer()->GetAspectRatio();
			float aspectRatioY = aspectRatioX < (16.0f / 9.0f) ? aspectRatioX / (16.0f / 9.0f) : 1.0f;

			DirectX::XMStoreFloat4x4(
				&scene.m_vertexShaderconstantBufferData.projection,
				DirectX::XMMatrixTranspose(
					DirectX::XMMatrixPerspectiveFovRH(
						2.0f * std::atan(std::tan(DirectX::XMConvertToRadians(70) * 0.5f) / aspectRatioY),
						aspectRatioX,
						0.01f,
						100.0f
					)
				)
			);
		}


	private:
		HWND m_nativeWindowHandle;		
	};

}; // end: namespace Rldx
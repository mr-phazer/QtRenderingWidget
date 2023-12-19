#pragma once

#include <memory>
#include <windows.h>

#include "DXSwapChain.h"
#include "..\..\Interfaces\IDrawable.h"
#include "DxCameraOrbital.h"
#include "..\SceneGraph\SceneNodes\DxMeshNode.h"
#include "..\Types\ConstBuffers\CPUConstBuffers.h"
#include "..\Helpers\DxMeshCreator.h"

namespace Rldx {
	
	class DxScene : public IDrawable, IIdentifiable, IResizable
	{
	public:	
		using UniquePtr = std::unique_ptr<DxScene>;

	public:
		DxScene(const std::string& name = "") : IIdentifiable(name) {};
		virtual std::string GetTypeString() { return "DxScene"; }

		virtual void Draw(
			ID3D11DeviceContext* poDeviceContext) override;
		// TODO: needed? all this happens in the DxSceneCreator
		//virtual void Init(ID3D11Device* poDevice, HWND nativeWindowHandle);

		DxSceneNode* GetRootNode();
		void DeleteNode(DxSceneNode* node);

		DxSwapChain* GetSwapChain() const
		{
			return m_spoSwapChain.get();
		}

		DxSwapChain::UPtr& GetRefSwapChain()
		{
			return m_spoSwapChain;
		}
		
		LRESULT WINAPI NativeWindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{

			auto ret =  m_globalCamera.HandleMessages(hWnd, uMsg, wParam, lParam);
			
				return ret;
		}

		void AllocVertexShaderConstBuffer(ID3D11Device* poDevice);

		void Reset(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, unsigned int width, unsigned int height) override
		{			
			// TODO: should this be here? or in the swapchain? Or somewhere else
			D3D11_VIEWPORT viewPort;
			viewPort.TopLeftX = 0;
			viewPort.TopLeftY = 0;
			viewPort.Width = (FLOAT)width;
			viewPort.Height = (FLOAT)height;
			viewPort.MinDepth  = 0.0f;
			viewPort.MaxDepth = 1.0f;

			poDeviceContext->RSSetViewports(1, &viewPort);
		}

	private:
		void UpdateViewAndPerspective()
		{
			// TODO: view should do be done with a "global camera", using a general orbital camera
			// Use DirectXMath to create view and perspective matrices.
			
			m_vertexShaderconstantBufferData.eyePosition = { 0.2f, 1.0f, 0.8 };

			DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.f);

			//DirectX::XMStoreFloat4x4(
			//	&m_vertexShaderconstantBufferData.view,
			//	DirectX::XMMatrixTranspose(
			//		DirectX::XMMatrixLookAtRH(
			//			DirectX::XMLoadFloat3(&m_vertexShaderconstantBufferData.eyePosition),
			//			at,
			//			up
			//		)
			//	)
			//);

			m_vertexShaderconstantBufferData.view = m_globalCamera.GetViewMatrix();


			float aspectRatioX = GetSwapChain()->GetBackBuffer()->GetAspectRatio();
			float aspectRatioY = aspectRatioX < (16.0f / 9.0f) ? aspectRatioX / (16.0f / 9.0f) : 1.0f;

			DirectX::XMStoreFloat4x4(
				&m_vertexShaderconstantBufferData.projection,
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

		void UpdateVSConstBuffer(ID3D11DeviceContext* poDeviceContext)
		{
			m_vertexShaderConstantBuffer.SetData(poDeviceContext, m_vertexShaderconstantBufferData);
			ID3D11Buffer* vertexShaderSceneConstBuffers[1] = { m_vertexShaderConstantBuffer.GetBuffer() };
			poDeviceContext->VSSetConstantBuffers(0, 1, vertexShaderSceneConstBuffers);
		}
		
	private:
		DxSwapChain::UPtr m_spoSwapChain; // back buffer
		DxSceneNode::Sptr m_spoRootNode = DxSceneNode::Create("RootNode");
		std::unique_ptr<DirectX::CommonStates> m_upoCommonStates;

		// TODO: iterate over these, call IResizable::rest(width, height)
		std::vector<IResizable*> m_resizableObjects; // buffers/etc, that need to be resized when the window is resized
		
		// TODO: should this be 2 member? think about integrated solution
		VS_SceneConstantBuffer m_vertexShaderconstantBufferData; // constant buffer data for the vertex shader
		DirectX::ConstantBuffer<VS_SceneConstantBuffer> m_vertexShaderConstantBuffer; // constant buffer for the vertex shader

		DxCameraOrbital m_globalCamera;
	};


	class IDxSceneCreator
	{
	public:
		virtual DxScene::UniquePtr Create(ID3D11Device* poDevice, const std::string& name = "") = 0;
		

	};

	class NativeWindowSceneCreator : IDxSceneCreator
	{
		NativeWindowSceneCreator() {};	

	public:
		NativeWindowSceneCreator(HWND nativeWindowHandle) : m_nativeWindowHandle(nativeWindowHandle) {};
		
		DxScene::UniquePtr Create(ID3D11Device* poDevice, const std::string& name = "") override
		{
			auto newScene = std::make_unique<DxScene>();	

			RECT windowRect;
			GetWindowRect(m_nativeWindowHandle, &windowRect);

			UINT width = windowRect.right - windowRect.left;
			UINT height = windowRect.bottom - windowRect.top;

			// create swap chain
			newScene->GetRefSwapChain() = DxSwapChain::CreateForHWND(poDevice, m_nativeWindowHandle, width, height);
			
			// create VS constbuffer
			newScene->AllocVertexShaderConstBuffer(poDevice);

			return std::move(newScene);
		};

	private:		
		HWND m_nativeWindowHandle = static_cast<HWND>(0);		
	};

}; // end: namespace Rldx
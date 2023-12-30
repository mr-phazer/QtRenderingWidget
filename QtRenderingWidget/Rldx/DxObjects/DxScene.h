#pragma once

#include <memory>
#include <windows.h>


#include "..\..\Interfaces\IDrawable.h"
#include "..\Helpers\DxMeshCreator.h"
#include "..\SceneGraph\BaseNode\DxBaseNode.h"
#include "..\SceneGraph\SceneGraph.h"
#include "..\Types\ConstBuffers\CPUConstBuffers.h"
#include "DxCameraOrbital.h"
#include "DXSwapChain.h"
#include "DxConstBuffer.h"

namespace Rldx {

	enum class DxSceneTypeEnum
	{
		Normal
	};

	class DxScene : public TIdentifiable<DxSceneTypeEnum>, IResizable, IDrawable, IUpdateable
	{
	public:
		using UniquePtr = std::unique_ptr<DxScene>;

	public:
		DxScene(const std::string& name = "") { 
			m_name = name; 
			
		};

		void Init(ID3D11Device* poDevice);

		std::string GetTypeString() const override { return "DxScene"; }
		DxSceneTypeEnum GetType() const override { return DxSceneTypeEnum::Normal; }

		virtual void Update(float timeElapsed) override;
		virtual void Draw(ID3D11DeviceContext* poDeviceContext) override;
		
		

		DxBaseNode* GetRootNode();
		void DeleteNode(DxBaseNode* node);

		DxSwapChain* GetSwapChain() const
		{
			return m_spoSwapChain.get();
		}

		DxSwapChain::UPtr& GetRefSwapChain()
		{
			return m_spoSwapChain;
		}

		LRESULT WINAPI NativeWindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		

		

		virtual void Resize(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, unsigned int width, unsigned int height) override;

	private:
		void UpdateViewAndPerspective();
		void UpdateAndBindVSConstBuffer();
		void DEBUGGING_SetViewAndPerspective()
		{
			// Use DirectXMath to create view and perspective matrices.

			DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.7f, 1.5f, 0.f);
			DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, -0.1f, 0.0f, 0.f);
			DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.f);

			DirectX::XMStoreFloat4x4(
				&m_sceneFrameVSConstBuffer.data.view,
				DirectX::XMMatrixTranspose(
					DirectX::XMMatrixLookAtRH(
						eye,
						at,
						up
					)
				)
			);

			m_sceneFrameVSConstBuffer.data.view = sm::Matrix::Identity;

			float aspectRatioX = m_spoSwapChain->GetBackBuffer()->GetAspectRatio();
			float aspectRatioY = aspectRatioX < (16.0f / 9.0f) ? aspectRatioX / (16.0f / 9.0f) : 1.0f;

			DirectX::XMStoreFloat4x4(
				&m_sceneFrameVSConstBuffer.data.projection,
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

		//------------------------
	private:
		// TODO: remove?
		//SceneGraphParser m_sceneGraphParser;
		//DxSceneNode::SharedPointer m_spoRootNode = DxSceneNode::Create("RootNode");

		DxSceneGraph m_sceneGraph;
		DxStandardRenderQueue m_renderQueue;

		DxSwapChain::UPtr m_spoSwapChain; // back buffer
		
		
		std::unique_ptr<DirectX::CommonStates> m_upoCommonStates;

		// TODO: iterate over these, call IResizable::rest(width, height)
		std::vector<IResizable*> m_resizableObjects; // buffers/etc, that need to be resized when the window is resized

		// TODO: should this be 2 member? think about integrated solution
		TConstBuffer<VS_PerScene_ConstantBuffer> m_sceneFrameVSConstBuffer;
		TConstBuffer<LightStruct> m_sceneFramePSConstBuffer;

		DxCameraOrbital m_globalCamera;
		DxCameraOrbital m_globalLighting;
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
			newScene->Init(poDevice);
			return std::move(newScene);
		};

	private:
		HWND m_nativeWindowHandle = static_cast<HWND>(0);
	};
};
#pragma once

#include <memory>
#include <windows.h>

#include "DXSwapChain.h"
#include "..\..\Interfaces\IDrawable.h"
#include "DxCameraOrbital.h"
#include "..\SceneGraph\SceneNodes\DxMeshNode.h"
#include "..\Types\ConstBuffers\CPUConstBuffers.h"
#include "..\Helpers\DxMeshCreator.h"
#include "..\SceneGraph\Helpers\SceneGraphParser.h"

namespace Rldx {
	
	enum class DxSceneTypeEnum
	{
		Normal
	};

	class DxScene : public IDrawable, TIdentifiable<DxSceneTypeEnum>, IResizable
	{
	public:	
		using UniquePtr = std::unique_ptr<DxScene>;

	public:
		DxScene(const std::string& name = "") { m_name = name; };
				
		std::string GetTypeString() const override { return "DxScene"; }
		DxSceneTypeEnum GetType() const override { return DxSceneTypeEnum::Normal; }

		virtual void Draw(
			ID3D11DeviceContext* poDeviceContext) override;
		// TODO: needed? all this happens in the DxSceneCreator
		virtual void Init(ID3D11Device* poDevice, HWND nativeWindowHandle)
		{
			//m_globalCamera.SetProjParams(DirectX::XM_PI / 4, 1.0f, 1.0f, 1000.0f);
			
			map<int, std::string> testMap = {{1, "one"}, {2, "two"}, {3, "three"}};

			auto& stuff = testMap[10];
		}

		IDxSceneNode* GetRootNode();
		void DeleteNode(IDxSceneNode* node);

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

		void DoFrameMovement(float elapsedTime);

		void AllocVertexShaderConstBuffer(ID3D11Device* poDevice);

		virtual void Reset(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, unsigned int width, unsigned int height) override;

	private:
		void UpdateViewAndPerspective();

		void UpdateVSConstBuffer(ID3D11DeviceContext* poDeviceContext);
		
	private:
		SceneGraphParser m_sceneGraphParser;
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
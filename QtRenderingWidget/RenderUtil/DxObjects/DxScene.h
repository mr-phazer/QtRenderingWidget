
#pragma once

#include "DXSwapChain.h"
#include "IDrawable.h"
#include "..\SceneGraph\SceneNodes\DxSceneNode.h"

namespace Rldx {

	
	class DxScene : public IDrawable, public TSmartPointerAll<DxScene>
	{
	
	public:	
		

	public:
		virtual void Draw(
			ID3D11DeviceContext* poDeviceContext,
			ID3D11RenderTargetView* destRtV = nullptr,
			IDxShaderProgram* shaderProgram = nullptr) override
		{
			/*
				- first test, draw one simple mesh.


				- process scene graph to get all meshnodes
				- put mesh nodes in a container "class DxRenderQueue"
				- for each mesh node in DxRenderQueue
				  - user DxRenderPass::Draw(DxRenderQueue queueToDraw)




			*/

		};

		DxSceneNode* GetRootNode();
		void DeleteNode(DxSceneNode* node);

		DxSwapChain::UPtr& GetRefSwapChain()
		{
			return m_swapChain;
		}

	private:
		DxSwapChain::UPtr m_swapChain; // back buffer
		DxSceneNode::Sptr m_spoRootNode = DxSceneNode::Create("RootNode");

		std::vector<IResizable*> m_resizableObjects; // buffers/etc, that need to be resized when the window is resized
	};


	class IDxSceneCreator
	{
	public:
		DxScene::UniquePtr Create(ID3D11Device* poDevice);
		
	};

	class BoundToHWNDSceneCreator
	{
		HWND m_nativeWindowHandle;
	public:
		DxScene::UniquePtr Create(ID3D11Device* poDevice, HWND nativeWindowHandle)
		{
			auto newScene = DxScene::MakeUnique();
			newScene->GetRefSwapChain() = DxSwapChain::CreateForHWND(poDevice, nativeWindowHandle);
		};

		BoundToHWNDSceneCreator(HWND nativeWindowHandle) : m_nativeWindowHandle(nativeWindowHandle) {};
	};

}; // end: namespace Rldx
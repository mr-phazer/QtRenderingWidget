
#pragma once

#include "DXSwapChain.h"
#include "IDrawable.h"
#include "..\SceneGraph\SceneNodes\DxSceneNode.h"

namespace Rldx {
	
	class DxScene : public IDrawable
	{
	public:
		using UPtr = std::unique_ptr<DxSwapChain>;
	public:		
		virtual void Draw(
			ID3D11DeviceContext* poDeviceContext,
			ID3D11RenderTargetView* destRtV = nullptr,
			IDxShaderProgram* shaderProgram = nullptr);

		DxSceneNode* GetRootNode();
		void DeleteNode(DxSceneNode* node);

		DxSwapChain::UPtr& GetRefSwapChain()
		{
			return m_swapChain;
		}

	private:
		DxSwapChain::UPtr m_swapChain; // back buffer
		DxSceneNode::Sptr m_spoRootNode = DxSceneNode::Create("RootNode");
	};


	class SceneCreator
	{
		DxScene::UPtr Create(HWND nativeWindowHandle, ID3D11Device* poDevice)
		{
			auto newScene = std::make_unique<DxScene>();
			newScene->GetRefSwapChain() = DxSwapChain::CreateForHWND(poDevice, nativeWindowHandle);
		}
	};

}; // end: namespace Rldx
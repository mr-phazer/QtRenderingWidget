
#pragma once

#include "DXSwapChain.h"
#include "IDrawable.h"
#include "..\SceneGraph\SceneNodes\DxSceneNode.h"

namespace Rldx {

	class DxScene : public IDrawable
	{
	public:
		using UPtr = std::unique_ptr<DxScene>;
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
				- user DxRenderPass::Draw(DxRenderQueue queueToDraw)
				- for each mesh node in DxRenderQueue
					- set shader program - from material if any, else the shader program passed in this method

					- set vertex buffer
					- set index buffer

					- set texture - from material if existing - else default textures
					- set mesh vertex shader constant buffer - mesh transform, animation
					- set mesh pixel shader constant buffer - lighting params, colors, etc
					- draw
			
			*/

		}



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


	class SceneCreator
	{
	public:
		static DxScene::UPtr Create(ID3D11Device* poDevice, HWND nativeWindowHandle)
		{
			auto newScene = std::make_unique<DxScene>();
			newScene->GetRefSwapChain() = DxSwapChain::CreateForHWND(poDevice, nativeWindowHandle);
		}
	};

}; // end: namespace Rldx
#pragma once

// qt
#include <qobject.h>

// athour
#include "..\DxObjects\DxScene.h"
#include "..\DxObjects\IDrawable.h"

#include "..\..\..\DirectXTK\Inc\CommonStates.h"


namespace Rldx {	

	class DxSceneManager
	{	
	public:	
		using UniquePtr = std::unique_ptr<DxSceneManager>;
		
		static UniquePtr Create(ID3D11Device* poDevice, HWND nativeWindowHandle)
		{				
			auto newSceneManager = std::make_unique<DxSceneManager>();
						
			newSceneManager->m_spoCurrentScene = newSceneManager->CreateScene(poDevice, nativeWindowHandle);

			return std::move(newSceneManager);
		}

		DxScene::UniquePtr CreateScene(ID3D11Device* poDevice, HWND nativeWindowHandle)
		{
			NativeWindowSceneCreator sceneCreator(nativeWindowHandle);
			auto newScene = sceneCreator.Create(poDevice);
			newScene->Init(poDevice, nativeWindowHandle);

			return std::move(newScene);
		}

		void OnUpdateFrame();
		void OnResize();

		DxScene* GetCurrentScene()
		{
			return m_spoCurrentScene.get();
		}


	private:
		DxScene::UniquePtr m_spoCurrentScene;
	};



}; // namespace Rldx

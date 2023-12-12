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

			BoundToHWNDSceneCreator sceneCreator(nativeWindowHandle);			
			newSceneManager->m_spoScene = sceneCreator.Create(poDevice);

			return std::move(newSceneManager);
		}

		void OnUpdateFrame();
		void OnResize();

		DxScene* GetScene()
		{
			return m_spoScene.get();
		}


	private:
		DxScene::UniquePtr m_spoScene;
	};



}; // namespace Rldx

#pragma once

#include <qobject.h>

#include "..\DxObjects\DxScene.h"
#include "..\DxObjects\IDrawable.h"


namespace Rldx {	

	class DxSceneManager
	{	
	public:	
		using UniquePtr = std::unique_ptr<DxSceneManager>;
		
		static UniquePtr Create(ID3D11Device* poDevice, HWND nativeWindowHandle)
		{				
			auto newSceneManager = std::make_unique<DxSceneManager>();

			BoundToHWNDSceneCreator sceneCreator(nativeWindowHandle);			
			newSceneManager->m_scene = sceneCreator.Create(poDevice);

			return std::move(newSceneManager);
		}

		void OnUpdateFrame();
		void OnResize();

	private:
		DxScene::UniquePtr m_scene;
	};



}; // namespace Rldx

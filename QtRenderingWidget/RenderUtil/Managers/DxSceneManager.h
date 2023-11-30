#pragma once

#include <qobject.h>

#include "..\DxObjects\DxScene.h"
#include "..\DxObjects\IDrawable.h"
#include "..\..\Common\TSmartPointer.h"

namespace Rldx {	

	class DxSceneManager
	{	
	public:	
		using PUnique = std::unique_ptr<DxSceneManager>;
		
		static PUnique Create()
		{				
			auto newSceneManager = std::make_unique<DxSceneManager>();
			newSceneManager->m_scene = HWNDSceneCreator::Create(nullptr, 0);

			return std::move(newSceneManager);
		}

		void OnUpdateFrame();
		void OnResize();

	private:
		DxScene::UPtr m_scene;
	};



}; // namespace Rldx

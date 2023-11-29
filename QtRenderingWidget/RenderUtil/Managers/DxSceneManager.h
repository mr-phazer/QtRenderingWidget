#pragma once

#include <qobject.h>

#include "..\DxObjects\DxScene.h"
#include "..\DxObjects\IDrawable.h"
#include "..\..\Common\TSmartPointer.h"

namespace Rldx {	

	class DxSceneManager
	{	
	public:	
		using Ptr = TUniquePtr<DxSceneManager>;
		
		static Ptr::UniquePtr Create()
		{	
			return Ptr::MakeUnique();
		}

		void OnUpdateFrame();
		void OnResize();

	private:
		DxScene::UPtr m_scene;
	};



}; // namespace Rldx

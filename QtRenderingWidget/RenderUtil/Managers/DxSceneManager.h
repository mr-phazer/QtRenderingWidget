#pragma once

#include <qobject.h>

#include "..\DxObjects\DxScene.h"
#include "..\DxObjects\IDrawable.h"


namespace Rldx {	

	class DxSceneManager
	{
		Q_OBJECT		

	private:
		std::unique_ptr<DxScene> m_scene;
	};


}; // namespace Rldx

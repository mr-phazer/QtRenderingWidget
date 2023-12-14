#pragma once

// qt
#include <qobject.h>

// athour
#include "..\DxObjects\DxScene.h"
#include "..\DxObjects\IDrawable.h"

#include "..\..\..\DirectXTK\Inc\CommonStates.h"


namespace Rldx {	

	class DxSceneManager : public IResizable
	{	
	public:	
		using UniquePtr = std::unique_ptr<DxSceneManager>;
		
		static DxSceneManager::UniquePtr Create(ID3D11Device* poDevice)
		{				
			auto newSceneManager = std::make_unique<DxSceneManager>();
			return std::move(newSceneManager);
		}

		void SetScene(DxScene::UniquePtr& scene)
		{
			m_spoCurrentScene = std::move(scene);
		}		

		void OnUpdateFrame();
		
		virtual void Reset(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, unsigned int width, unsigned int height) override
		{
			if (m_spoCurrentScene)
			{
				m_spoCurrentScene->Reset(poDevice, poDeviceContext, width, height);
			}
		}
		


		DxScene* GetCurrentScene()
		{
			return m_spoCurrentScene.get();
		}


	private:
		DxScene::UniquePtr m_spoCurrentScene;
	};



}; // namespace Rldx

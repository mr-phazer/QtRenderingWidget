#pragma once

// qt
#include <qobject.h>

// athour
#include "..\DxObjects\DxScene.h"
#include "..\..\Interfaces\IDrawable.h"
#include "..\..\Interfaces\IResizable.h"

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
		
		LRESULT WINAPI NativeWindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)		
		{		
			if (m_spoCurrentScene != nullptr)
			{
				return m_spoCurrentScene->NativeWindowProcedure(hWnd, uMsg, wParam, lParam);
			}
		}


		DxScene* GetCurrentScene()
		{
			return m_spoCurrentScene.get();
		}


	private:
		DxScene::UniquePtr m_spoCurrentScene = nullptr;		
	};



}; // namespace Rldx

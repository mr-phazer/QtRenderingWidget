#pragma once
#include "..\Rendering\DxScene.h"

#include <Timer\SystemClockChecker.h>

namespace rldx {

	class DxSceneManager : public IResizable
	{
		DxResourceManager m_localResourceManager;
		std::unique_ptr<DxScene> m_upoCurrentScene;
		timer::SystemClockChecker m_systemClock;
		bool m_bRenderingRunning = false;

	public:
		using UniquePtr = std::unique_ptr<DxSceneManager>;

		static DxSceneManager::UniquePtr Create(ID3D11Device* poDevice)
		{
			auto newSceneManager = std::make_unique<DxSceneManager>();
			return std::move(newSceneManager);
		}

		void SetScene(std::unique_ptr<DxScene>& newScene)
		{
			m_upoCurrentScene = std::move(newScene);
		}

		void MoveFrame()
		{
			if (m_upoCurrentScene && IsRenderRunning())
			{
				m_upoCurrentScene->Update(m_systemClock.GetLocalTime());
			}
		}

		virtual void Resize(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, unsigned int width, unsigned int height) override
		{
			if (m_upoCurrentScene)
			{
				m_upoCurrentScene->Resize(poDevice, poDeviceContext, width, height);
			}
		}

		/// <summary>
		/// Send Native Window events to a DxScene, as the low-level camera impl needs them
		/// </summary>		
		LRESULT WINAPI ForwardNativeWindowEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			if (m_upoCurrentScene)
			{
				return m_upoCurrentScene->ForwardNativeWindowEvent(hWnd, uMsg, wParam, lParam);
			}

			return false;
		}

		DxScene* GetCurrentScene()
		{
			return m_upoCurrentScene.get();
		}

		rldx::DxResourceManager& GetResourceManager()
		{
			return m_localResourceManager;
		}

		bool IsRenderRunning() const { return m_bRenderingRunning; }
		void SetRenderRunningState(bool state) { m_bRenderingRunning = state; }
	};

}; // namespace rldx
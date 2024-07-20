#pragma once

#include <DirectXTK\Inc\CommonStates.h>

#include <CommonLibs\Timer\SystemClockChecker.h>
#include "..\Rendering\DxScene.h"

namespace rldx {

	class DxSceneManager : public IResizable
	{

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

		bool IsRenderRunning() const { return m_bRenderingRunning; }
		void SetRenderRunningState(bool state) { m_bRenderingRunning = state; }

	private:
		std::unique_ptr<DxScene> m_upoCurrentScene = nullptr;
		timer::SystemClockChecker m_systemClock;
		bool m_bRenderingRunning = false;
	};

}; // namespace rldx

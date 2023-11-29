#pragma once

#include <memory>

#include <qwidget.h>
#include <qtimer.h>

#include "..\Managers\DxDeviceManager.h"



namespace Rldx
{
	class DxTempDemoRenderer : public QObject
	{
		Q_OBJECT

	public:
		typedef std::unique_ptr<DxTempDemoRenderer> UPtr;

	public:
		// TODO: implemement
		//static std::unique_ptr<DxSceneRenderer> create(Rldx::DxDeviceManager* _spManager, QWidget* poRenderView);

		void OnRenderFrameTimerHandler(Rldx::DxDeviceManager* poDxManager)
		{
			if (!m_bRenderingRunning)
				return;


			m_spoSwapChain->GetBackBuffer()->SetToActiveTargetView(poDxManager->GetDeviceContext());

			// TODO: remove if works
			//m_poScreenBuffer->SetToActiveTargetView(poDxManager->deviceContext());



			sm::Vector4	v4Color =
			{
				static_cast<float>(rand() % 255),
				static_cast<float>(rand() % 255),
				static_cast<float>(rand() % 255),
				255.0f
			};

			v4Color /= 255.0f;

			m_spoSwapChain->GetBackBuffer()->ClearPixels(poDxManager->deviceContext(), v4Color);

			// TODO: remove if works
			//m_poScreenBuffer->clearPixels(poDxManager->deviceContext(), v4Color);

			// TODO: PARSE scenegraph
			// TOOS: draw resulatant queue



			poDxManager->RenderText();

			m_spoSwapChain->Present(poDxManager->deviceContext());
		}

		static DxTempDemoRenderer::PDXScene CreateScene(Rldx::DxDeviceManager* pManager, QWidget* poRenderView)
		{
			auto poScene = PDXScene(new DxTempDemoRenderer);

			// create swap chain for supplied QWdiget
			poScene->m_spoSwapChain =
				DxSwapChain::CreateForHWND(
					pManager->GetDevice(),
					reinterpret_cast<HWND>(poRenderView->winId()),
					poRenderView->width(),
					poRenderView->height());

			poScene->updateViewPort(pManager->deviceContext(), poRenderView);

			

			poScene->startRendering(pManager);

			return poScene;
		}

		void startRendering(Rldx::DxDeviceManager* poDxManager)
		{		
			m_qtTimer = new QTimer(this);

			connect(m_qtTimer, &QTimer::timeout, [this, poDxManager]()
				{
					OnRenderFrameTimerHandler(poDxManager);
				}
			);

			m_qtTimer->start(1000.0 / m_fps);
		}

		void updateViewPort(ID3D11DeviceContext* _pDeviceContext, QWidget* _renderView)
		{
			//// Set viewport
			m_oViewPort.TopLeftX = 0;
			m_oViewPort.TopLeftY = 0;
			m_oViewPort.Width = (FLOAT)_renderView->width();
			m_oViewPort.Height = (FLOAT)_renderView->height();
			m_oViewPort.MinDepth = 0.f;
			m_oViewPort.MaxDepth = 1.f;
			_pDeviceContext->RSSetViewports(1, &m_oViewPort);

		}





		bool init(const ID3D11Device* poDevice);

		void draw(const ID3D11DeviceContext* poDeviceContect);





		//Rldx::SceneGraph m_scenGraph;

		void SetRenderingRunning(bool state) {
			m_bRenderingRunning = state;
		}

		bool GetRenderingRunningState() const
		{
			return m_bRenderingRunning;
		}


	private:
		Rldx::DxSwapChain::SPtr m_spoSwapChain;
		//Rldx::DxTexture* m_poScreenBuffer;

	private:
		QTimer* m_qtTimer;
		bool m_bRenderingRunning = true;
		float m_fps = 60.f;
		D3D11_VIEWPORT m_oViewPort;

		//Rldx::RendererQueue m_renderingQueue;
	};

}

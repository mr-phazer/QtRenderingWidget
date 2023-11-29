#pragma once

#include <QWidget.h>
#include <qstyle.h>
#include <qstyleoption.h>
#include <qmessagebox.h>
#include <qtimer.h>											   

#include "ui_QtRenderingWidget.h"

#include "..\..\RenderUtil\Managers\DxDeviceManager.h"
#include "..\..\..\DirectXTK\Inc\SpriteFont.h"
#include "..\..\RenderUtil\Managers\DxSceneManager.h"

namespace Rldx
{

	class QtRenderingWidget : public QWidget, public Ui::QtRenderingWidgetClass
	{
		Q_OBJECT

	public:
		QtRenderingWidget(QWidget* parent = Q_NULLPTR);

		Rldx::DxTempDemoRenderer::Uptr m_upoScene;-
		Rldx::DxDeviceManager* m_spDxManager;

		//void setSwapChain(Rldx::DXSwapChain::sptrDXSwapChain _pSwapChain)
		//{
		//	spSwapChain = std::move(_pSwapChain); // move the ownership of the swap chain ptr
		//};

		bool Init(Rldx::DxDeviceManager* dxManager)
		{
			/*	fontEngine.m_font = std::make_unique<SpriteFont>(_dx->device(), L"myfile.spritefont");
				fontEngine.m_spriteBatch = std::make_unique<SpriteBatch>(_dx->deviceContext());

				if (!(fontEngine.m_font && fontEngine.m_spriteBatch))
				{
					throw exception("Error Loading Font");
				}*/
			m_spDxManager = dxManager;
			m_upoScene = Rldx::DxTempDemoRenderer::CreateScene(m_spDxManager, this);

			return true;
		}

		//void timerHandler(const Rldx::DXD3DManager::spDXD3DManger& _dxManager)
		//{
		//	if (!m_bRenderingRunning)
		//		return;

		//	renderText(_dxManager);
		//	spSwapChain->present(_dxManager->deviceContext());
		//}

		//void startRendering(const Rldx::DXD3DManager::spDXD3DManger& _dxManager, int _FPS = 60)
		//{
		//	m_timer = new QTimer(this);

		//	connect(m_timer, &QTimer::timeout, [&]()
		//			{
		//				timerHandler(_dxManager);
		//			}
		//	);

		//	m_timer->start(1000 / _FPS);
		//}





	//	void renderText(const Rldx::DXD3DManager::spDXD3DManger& _dxManager)
	//	{
	//
	//		spSwapChain->getBackBuffer().clearPixels(_dxManager->deviceContext(), spSwapChain->Color);
	//		_dxManager->deviceContext()->OMSetRenderTargets(1, spSwapChain->getBackBuffer().getAddressOfRenderTargetView(), nullptr);
	//	
	//
	//
	//		
	//
	//		DirectX::SimpleMath::Vector2 origin = { 0,0 }; // m_font->MeasureString(output) / 2.f;
	//
	//		DirectX::SimpleMath::Vector3 vScale = { 5.f, 5.f, 5.f };
	//		DirectX::SimpleMath::Vector2 vPosition = { 0.f, 0.f };
	//		DirectX::SimpleMath::Color vColor = { 0.0f, 1.0f, 1.0f, 0.0f };
	//		float rotation = 0.0f;
	//
	//		// Create an alpha enabled blend state description.
	//		//const float blendFactor[4] = {1,1,1,1};
	//		//m_poScene->getDeviceContext()->OMSetBlendState(poBlendState, blendFactor, 0xFFFFFFFF);
	//
	//		_dxManager->deviceContext()->OMSetDepthStencilState(_dxManager->depthStencilStates.Off.Get(), 1);
	//		
	//
	//		fontEngine.m_spriteBatch->Begin();			   
	//		for (auto& itStr : fontEngine.m_stringsToDraw)
	//		{
	//			auto boundRect = fontEngine.m_font->MeasureDrawBounds(itStr.c_str(), vPosition, false);
	//
	//			fontEngine.m_font->DrawString(
	//				fontEngine.m_spriteBatch.get(),
	//				itStr.c_str(),
	//				vPosition,
	//				vColor,
	//				rotation,
	//				origin,
	//				vScale,
	//				DirectX::SpriteEffects::SpriteEffects_None
	//			);
	//
	//			vPosition.y += boundRect.bottom;
	//		}
	//
	//		//float y = Vector3(m_font->MeasureString(TextOutDebug::strStringToRender.c_str())).y;
	//
	//		//for (auto& it : TextOutDebug::vecStringsToRender) {
	//		//	/*m_font->DrawString(m_spriteBatch.get(), it.c_str(),
	//		//		{ 0,y }, vColor, 0.f, origin, xmColor, DirectX::SpriteEffects::SpriteEffects_None);*/
	//
	//		//	auto tempColor = vColor;
	//		//	tempColor.w = it.second;
	//		//	tempColor.x = it.second;
	//
	//		//	m_font->DrawString(
	//		//		m_spriteBatch.get(),
	//		//		(it.first + ": alpha: " + to_string(it.second)).c_str(),
	//		//		{ 0, y },
	//		//		tempColor,
	//		//		rotation,
	//		//		origin,
	//		//		vScale,
	//		//		DirectX::SpriteEffects::SpriteEffects_None
	//		//	);
	//
	//		//	if (it.second > 0)
	//		//	{
	//		//		it.second -= 0.01;
	//		//	}
	//
	//		//	y += Vector3(m_font->MeasureString(it.first.c_str())).y;;
	//		//}
	//
	//		fontEngine.m_spriteBatch->End();
	//
	//		_dxManager->deviceContext()->OMSetDepthStencilState(_dxManager->depthStencilStates.On.Get(), 1);
	//
	//		//if (m_bRenderingRunning)
	//		//{
	//		//	updatePS_Light_ConstBuffer_PerFrame();
	//		//	update_VS_ConstBuffer_PerFrame();
	//
	//		//	RendererQueue m_meshRenderQueue;
	//		//	SceneGraphParserService::getParsedMeshes(m_spoRootNode.get(), m_meshRenderQueue.nodes());
	//		//	spoRenderPass->performPass(this, &m_meshRenderQueue);
	//		//}
	//	
	//
	//
	//
	//
	////		deviceContext()->OMSetDepthStencilState(m_poDepthStencilState_On, 1);
	//	}
	//
	//
	//
	//	/*void paintEvent(QPaintEvent* event) override {
	//		QStyleOption opt;
	//		opt.init(this);
	//		QPainter p(this);
	//		p.setBackground(QBrush(Qt::blue));
	//		style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	//
	//		QWidget::paintEvent(event);
	//	}*/
	//
	//private:
	//	//bool m_bRenderingRunning = true;
	//	//Rldx::DXSwapChain::sptrDXSwapChain spSwapChain;
	//	//QTimer* m_timer;
	//
	//private:
	//	
	//
	//	struct FontEngine
	//	{
	//		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	//		std::unique_ptr<DirectX::SpriteFont> m_font;
	//
	//		std::vector<std::wstring> m_stringsToDraw = { L"D3D\nIt Works\n\r\NOW: Scenegrapdh" };
	//	} fontEngine;
	};

}; // namespace Rldx
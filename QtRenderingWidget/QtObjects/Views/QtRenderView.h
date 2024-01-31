#pragma once

#include <qwidget.h>
#include <qstyle.h>
#include <qstyleoption.h>
#include <qmessagebox.h>
#include <qtimer.h>											   
#include <qevent.h>
#include <mouse.h>

#include "..\..\..\DirectXTK\Inc\SpriteFont.h"

#include "ui_QtRenderView.h"

#include "..\rldx\rldx\Managers\DxDeviceManager.h"
#include "..\rldx\rldx\Managers\DxSceneManager.h"
#include "..\rldx\rldx\SceneGraph\SceneNodes\DxMeshNode.h"

#include "..\rldx\rldx\Rendering\DxShaderProgram.h"
#include "..\rldx\rldx\DataTypes\DxMeshData.h"

class QtRenderView : public QWidget, public Ui::QtRenderingViewWidgetClass
{

	// overriden methods
	void resizeEvent(QResizeEvent* event) override;
	QPaintEngine* paintEngine() const override;
	bool event(QEvent* event) override;
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

	LRESULT WINAPI NativeWindowProcedure(MSG* pMsg);


	Q_OBJECT

public:
	QtRenderView(QWidget* parent = Q_NULLPTR);

	

	bool Init(/*rldx::DxDeviceManager* dxManager*/);

	void FrameTimeOutHandler();
	void StartRendering(float _FPS = 60.0f)
	{
		m_upoSceneManager->SetRenderRunningState(true);
		
		// TODO: move this to scene manager?
		m_timer = new QTimer(this);

		connect(m_timer, &QTimer::timeout, [&]()
				{
					FrameTimeOutHandler();
				}
		);

		m_frameTime = 1000.0f / _FPS;
		m_timer->start(m_frameTime);
	}

	rldx::DxSceneManager& GetSceneManager() { return *m_upoSceneManager; }

signals:
	void keyPressed(QKeyEvent*);
	void mouseMoved(QMouseEvent*);
	void mouseClicked(QMouseEvent*);
	void mouseReleased(QMouseEvent*);
	void doubleclicked();
	void detachedWindowClose();

private:
	rldx::DxSceneManager::UniquePtr m_upoSceneManager;
	//rldx::DxDeviceManager* m_poDxManager = nullptr;
	
	QTimer* m_timer;
	float m_frameTime = 0;

};


//	void renderText(const rldx::DXD3DManager::spDXD3DManger& _dxManager)
//	{
//
//		spSwapCha:in->getBackBuffer().clearPixels(_dxManager->deviceContext(), spSwapChain->Color);
//		_dxManager->deviceContext()->OMSetRenderTargets(1, spSwapChain->getBackBuffer().getAddressOfRenderTargetView(), nullptr);
//	
//
//
//		
//
//		DirectX::SimpleMath::Vector2 origin = { 0,0 }; // m_upoFont->MeasureString(output) / 2.f;
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
//		fontEngine.m_upoSpriteBatch->Begin();			   
//		for (auto& itStr : fontEngine.m_stringRenderQueue)
//		{
//			auto boundRect = fontEngine.m_upoFont->MeasureDrawBounds(itStr.c_str(), vPosition, false);
//
//			fontEngine.m_upoFont->DrawString(
//				fontEngine.m_upoSpriteBatch.get(),
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
//		//float y = Vector3(m_upoFont->MeasureString(TextOutDebug::strStringToRender.c_str())).y;
//
//		//for (auto& it : TextOutDebug::vecStringsToRender) {
//		//	/*m_upoFont->DrawString(m_upoSpriteBatch.get(), it.c_str(),
//		//		{ 0,y }, vColor, 0.f, origin, xmColor, DirectX::SpriteEffects::SpriteEffects_None);*/
//
//		//	auto tempColor = vColor;
//		//	tempColor.w = it.second;
//		//	tempColor.x = it.second;
//
//		//	m_upoFont->DrawString(
//		//		m_upoSpriteBatch.get(),
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
//		//	y += Vector3(m_upoFont->MeasureString(it.first.c_str())).y;;
//		//}
//
//		fontEngine.m_upoSpriteBatch->End();
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
//
//private:
//	
//
//	struct FontEngine
//	{
//		std::unique_ptr<DirectX::SpriteBatch> m_upoSpriteBatch;
//		std::unique_ptr<DirectX::SpriteFont> m_upoFont;
//
//		std::vector<std::wstring> m_stringRenderQueue = { L"D3D\nIt Works\n\r\NOW: Scenegrapdh" };
//	} fontEngine;



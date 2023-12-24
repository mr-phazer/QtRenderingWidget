#pragma once

#include <qwidget.h>
#include <qstyle.h>
#include <qstyleoption.h>
#include <qmessagebox.h>
#include <qtimer.h>											   
#include <qevent.h>
#include <mouse.h>

#include "ui_QtRenderingWidget.h"

#include "..\..\Rldx\Managers\DxDeviceManager.h"
#include "..\..\..\DirectXTK\Inc\SpriteFont.h"
#include "..\..\Rldx\Managers\DxSceneManager.h"

class QtRenderingWidget : public QWidget, public Ui::QtRenderingWidgetClass
{
	Q_OBJECT

public:
	QtRenderingWidget(QWidget* parent = Q_NULLPTR);

	void resizeEvent(QResizeEvent* event) override
	{
		if (m_upoSceneManager)
		{
			m_timer->stop();
			m_upoSceneManager->Reset(m_poDxManager->GetDevice(), m_poDxManager->GetDeviceContext(), width(), height());
			m_timer->start();
		}	
	}

	//bool event(QEvent* event)
	//{

	//	switch (event->type())
	//	{
	//		// Workaround for https://bugreports.qt.io/browse/QTBUG-42183 to get key strokes.
	//		// To make sure that we always have focus on the widget when we enter the rect area.

	//	case QEvent::Leave:
	//	{
	//		::ReleaseCapture();
	//	}
	//	break;
	//	case QEvent::MouseButtonPress:
	//		emit mouseClicked((QMouseEvent*)event);

	//	case QEvent::FocusIn:
	//	case QEvent::FocusAboutToChange:
	//		if (::GetFocus() != reinterpret_cast<HWND>(this->winId()))
	//		{
	//			QWidget* nativeParent = this;
	//			while (true)
	//			{
	//				if (nativeParent->isWindow()) break;

	//				QWidget* parent = nativeParent->nativeParentWidget();
	//				if (!parent) break;

	//				nativeParent = parent;
	//			}

	//			if (nativeParent && nativeParent != this &&
	//				::GetFocus() == reinterpret_cast<HWND>(nativeParent->winId()))
	//				::SetFocus(reinterpret_cast<HWND>(this->winId()));
	//		}
	//		break;
	//	case QEvent::KeyPress:
	//		emit keyPressed((QKeyEvent*)event);
	//		break;
	//	case QEvent::MouseMove:
	//		emit mouseMoved((QMouseEvent*)event);
	//		break;
	//		/*case QEvent::MouseButtonPress:
	//			emit mouseClicked((QMouseEvent*)event);
	//			break;*/
	//	case QEvent::MouseButtonRelease:
	//		emit mouseReleased((QMouseEvent*)event);
	//		break;
	//	}

	//	return QWidget::event(event);
	//}



	//#if QT_VERSION >= 0x050000
	bool nativeEvent(const QByteArray& eventType,
		void* message,
		long* result)
	{
		Q_UNUSED(eventType);
		Q_UNUSED(result);
		
		if (m_upoSceneManager)
		{
			MSG* pMsg = reinterpret_cast<MSG*>(message);
			nativeWindowProcedure(pMsg);

			
		
		}

		return false;

		//return QWidget::nativeEvent(eventType, message, result);
	}

	LRESULT WINAPI nativeWindowProcedure(MSG* pMsg)
	{	
		return m_upoSceneManager->NativeWindowProcedure(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam);	
	}


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
		m_poDxManager = dxManager;		
		m_upoSceneManager = Rldx::DxSceneManager::Create(m_poDxManager->GetDevice());
		
		Rldx::NativeWindowSceneCreator sceneCreator(reinterpret_cast<HWND>(this->winId()));
		auto newScene = sceneCreator.Create(m_poDxManager->GetDevice(), "TestScene1");
		
		
		auto newPbrShaderProgram =
			Rldx::DxMeshShaderProgram::Create<Rldx::DxMeshShaderProgram>(
				m_poDxManager->GetDevice(),
				LR"(K:\Coding\repos\QtRenderingWidget_RPFM\x64\Debug\VS_Simple.cso)",
				LR"(K:\Coding\repos\QtRenderingWidget_RPFM\x64\Debug\PS_NoTextures.cso)"
			);				

		auto newSimpleShaderProgram =
			Rldx::DxMeshShaderProgram::Create<Rldx::DxMeshShaderProgram>(
				m_poDxManager->GetDevice(),
				LR"(K:\Coding\repos\QtRenderingWidget_RPFM\x64\Debug\VS_Simple.cso)",
				LR"(K:\Coding\repos\QtRenderingWidget_RPFM\x64\Debug\PS_Simple.cso)"
			);				

		auto meshNodeGrid = Rldx::DxMeshNode::Create("Grid");
		auto meshNodeCube = Rldx::DxMeshNode::Create("Cube");

		auto testMeshCube = Rldx::ModelCreator::MakeTestCubeMesh(m_poDxManager->GetDevice());
		meshNodeCube->SetMeshData(testMeshCube);
		meshNodeCube->SetShaderProgram(newPbrShaderProgram);

		auto testMeshGrid = Rldx::ModelCreator::MakeGrid(m_poDxManager->GetDevice(), 20);
		meshNodeGrid->SetMeshData(testMeshGrid);
		meshNodeGrid->SetShaderProgram(newSimpleShaderProgram);

		newScene->GetRootNode()->AddChild(meshNodeGrid);
		newScene->GetRootNode()->AddChild(meshNodeCube);
		
		m_upoSceneManager->SetScene(newScene);

		m_upoSceneManager->Reset(m_poDxManager->GetDevice(), m_poDxManager->GetDeviceContext(), width(), height());		

		return true;
	}

	void TimerHandler()
	{
		if (!m_bRenderingRunning)
			return;

//		renderText(_dxManager);		

		m_upoSceneManager->GetCurrentScene()->Draw(m_poDxManager->GetDeviceContext());	
		m_upoSceneManager->GetCurrentScene()->DoFrameMovement(m_frameTime);
	}
	

	void StartRendering(const Rldx::DxDeviceManager* dxManager, int _FPS = 100)
	{
		m_timer = new QTimer(this);

		connect(m_timer, &QTimer::timeout, [&]()
				{
					TimerHandler();
				}
		);

		m_frameTime = 1000.0f / _FPS;

		m_timer->start(1000 / _FPS);		
	}

signals:
	void keyPressed(QKeyEvent*);
	void mouseMoved(QMouseEvent*);
	void mouseClicked(QMouseEvent*);
	void mouseReleased(QMouseEvent*);
	void doubleclicked();

	void detachedWindowClose();

private:
	Rldx::DxSceneManager::UniquePtr m_upoSceneManager;
	Rldx::DxDeviceManager* m_poDxManager = nullptr;
	bool m_bRenderingRunning = true;	
	QTimer* m_timer;
	float m_frameTime = 0;

};


//	void renderText(const Rldx::DXD3DManager::spDXD3DManger& _dxManager)
//	{
//
//		spSwapCha:in->getBackBuffer().clearPixels(_dxManager->deviceContext(), spSwapChain->Color);
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



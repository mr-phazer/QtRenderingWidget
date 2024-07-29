#pragma once

#include <qevent.h>
#include <qmimedata.h>
#include <qtimer.h>											   
#include <qwidget.h>

#include "ui_QtRenderWidgetView.h"

#include <Rldx\Rldx\Creators\DxSceneCreator.h>
#include <Rldx\Rldx\Managers\DxDeviceManager.h>
#include <Rldx\Rldx\Managers\DxSceneManager.h>

#include "DirectXTK\Inc\SpriteFont.h"

#include "..\Controllers\QtRenderWidgetController.h"

class QtRenderWidgetView : public QWidget, public Ui::QtRenderingViewWidgetClass
{
	Q_OBJECT
private:
	QtRenderWidgetController* m_controller = nullptr;

	friend class QtRenderWidgetController;

private:
	// overriden methods
	void resizeEvent(QResizeEvent* event) override;
	void closeEvent(QCloseEvent* event) override;

	QPaintEngine* paintEngine() const override;
	bool event(QEvent* event) override;
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

	LRESULT WINAPI ForwardNativeWindowEvent(MSG* pMsg);

public:
	QtRenderWidgetView(QWidget* parent, const QString& gameidString);

	~QtRenderWidgetView()
	{
		TerminateRendering();
		emit this->WindowClosing();
	}

	void SetGrideDrawState(rldx::DxBaseNode::DrawStateEnum state)
	{
		m_upoSceneManager->GetCurrentScene()->SetGridState(state);
	}

	void PauseRendering()
	{
		m_upoSceneManager->SetRenderRunningState(false);
	}

	void ResumeRendering()
	{
		m_upoSceneManager->SetRenderRunningState(true);
	}

	void MakeScene()
	{
		// TODO: use abstract factor, based on incoming game ID
		auto sceneBuilder = std::make_shared<rldx::DxSceneCreator>();
		auto scene =
			sceneBuilder->Create((HWND)winId(),
								 rldx::DxDeviceManager::GetInstance().GetDevice(),
								 rldx::DxDeviceManager::GetInstance().GetDeviceContext(),
								 m_upoSceneManager->GetResourceManager(),
								 GetGameIdString().toStdWString());

		m_upoSceneManager->SetScene(scene);
	}

	void StartRendering(float _FPS = 60.0f);
	void TerminateRendering();

	rldx::DxSceneManager* GetSceneManager() { return m_upoSceneManager.get(); }
	QString GetGameIdString() const { return m_gameIdString; };
	void SetGameIdString(const QString& gameIdString) { m_gameIdString = gameIdString; }

private:
	void LoadExeResources(rldx::DxResourceManager& resourceManager, ID3D11Device* poDevice);
	bool InitRenderView();

protected:
	virtual void focusInEvent(QFocusEvent* event) override;
	virtual void focusOutEvent(QFocusEvent* event) override;

signals:
	void resizeEventHappend(QResizeEvent* event);
	void KeyPressed(QKeyEvent*);
	void mouseMoved(QMouseEvent*);
	void mouseClicked(QMouseEvent*);
	void mouseReleased(QMouseEvent*);
	void doubleclicked();
	void detachedWindowClose();
	void WindowClosing();

private:
	void DrawFrameHandler();
	void MakeConnections();

private:
	rldx::DxSceneManager::UniquePtr m_upoSceneManager;

	QTimer* m_timer = nullptr;
	float m_frameTime = 0;
	QString m_gameIdString;

	// only support drag+drog in debug mode


public:
#ifdef _DEBUG
	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);
#endif
};

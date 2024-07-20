#pragma once

#include <qevent.h>
#include <qtimer.h>											   
#include <qwidget.h>

#include "ui_QtRenderView.h"

#include <Rldx\Creators\DxSceneCreator.h>
#include <Rldx\Managers\DxDeviceManager.h>
#include <Rldx\Managers\DxSceneManager.h>

class QtRenderWidgetView;

class QtRenderController : public QObject
{
	Q_OBJECT;

	QtRenderWidgetView* view;

public:
	QtRenderController(QtRenderWidgetView* parentAndView);

public slots:
	void OnkeyPressed(QKeyEvent* keyEvent);
};

class QtRenderWidgetView : public QWidget, public Ui::QtRenderingViewWidgetClass
{
	Q_OBJECT
private:
	QtRenderController* m_controller = nullptr;

	friend class QtRenderController;

private:
	// overriden methods
	void resizeEvent(QResizeEvent* event) override;

	QPaintEngine* paintEngine() const override;
	bool event(QEvent* event) override;
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

	LRESULT WINAPI ForwardNativeWindowEvent(MSG* pMsg);

public:
	QtRenderWidgetView(QWidget* parent, const QString& gameidString);

	void SetGrideDrawState(rldx::DxBaseNode::DrawStateEnum state)
	{
		m_upoSceneManager->GetCurrentScene()->SetGridState(state);
	}

	bool InitRenderView();

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
								 GetGameIdString().toStdWString());

		m_upoSceneManager->SetScene(scene);
	}

	void StartRendering(float _FPS = 60.0f);

	rldx::DxSceneManager* GetSceneManager() { return m_upoSceneManager.get(); }
	QString GetGameIdString() const { return m_gameIdString; };
	void SetGameIdString(const QString& gameIdString) { m_gameIdString = gameIdString; }

private:
	void LoadExeResources(ID3D11Device* poDevice);

protected:
	virtual void focusInEvent(QFocusEvent* event) override;
	virtual void focusOutEvent(QFocusEvent* event) override;

signals:
	void resizeEventHappend(QResizeEvent* event);
	void keyPressed(QKeyEvent*);
	void mouseMoved(QMouseEvent*);
	void mouseClicked(QMouseEvent*);
	void mouseReleased(QMouseEvent*);
	void doubleclicked();
	void detachedWindowClose();

private:
	void FrameTimeOutHandler();
	void MakeConnections();

private:
	rldx::DxSceneManager::UniquePtr m_upoSceneManager;

	QTimer* m_timer = nullptr;
	float m_frameTime = 0;
	QString m_gameIdString;

	// only support drag+drog in debug mode

#ifdef _DEBUG
public:
	void dragEnterEvent(QDragEnterEvent* event)
	{
		event->acceptProposedAction();
	}

	void dropEvent(QDropEvent* event);
#endif
};
#include "QtRenderView.h"

//#include "..\..\Rldx\Rldx\Creators\DxSceneCreator.h"

//#include "..\RenderLib\Direct3d11Device.h"

using namespace rldx;

QtRenderWidgetView::QtRenderWidgetView(QWidget* parent)
	: QWidget(parent)
{
	setupUi(this);

	QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_NativeWindow);

    // Setting these attributes to our widget and returning null on paintEngine event
    // tells Qt that we'll handle all drawing and updating the widget ourselves.
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);



	/*this->setWindowTitle("QRenderenView (Testing) : Sence");

	setWindowFlag(Qt::Window, true);*/

	show();

	auto debug_1 = 1;
}

void QtRenderWidgetView::resizeEvent(QResizeEvent* event)
{
	if (m_upoSceneManager && m_upoSceneManager->IsRenderRunning())
	{
		//m_timer->stop();
		m_upoSceneManager->Resize(rldx::DxDeviceManager::GetInstance().GetDevice(), rldx::DxDeviceManager::GetInstance().GetDeviceContext(), width(), height());
		//m_timer->start();
	}
}
QPaintEngine* QtRenderWidgetView::paintEngine() const
{
	return Q_NULLPTR;
}

bool QtRenderWidgetView::event(QEvent* event)
{
	switch (event->type())
	{
		// Workaround for https://bugreports.qt.io/browse/QTBUG-42183 to get key strokes.
		// To make sure that we always have focus on the widget when we enter the rect area.

	case QEvent::Leave:
	{
		::ReleaseCapture();
	}
	break;
	case QEvent::MouseButtonPress:
		emit mouseClicked((QMouseEvent*)event);

	case QEvent::FocusIn:
	case QEvent::FocusAboutToChange:
		if (::GetFocus() != reinterpret_cast<HWND>(this->winId()))
		{
			QWidget* nativeParent = this;
			while (true)
			{
				if (nativeParent->isWindow()) break;

				QWidget* parent = nativeParent->nativeParentWidget();
				if (!parent) break;

				nativeParent = parent;
			}

			if (nativeParent && nativeParent != this &&
				::GetFocus() == reinterpret_cast<HWND>(nativeParent->winId()))
				::SetFocus(reinterpret_cast<HWND>(this->winId()));
		}
		break;
	case QEvent::KeyPress:
		emit keyPressed((QKeyEvent*)event);
		break;
	case QEvent::MouseMove:
		emit mouseMoved((QMouseEvent*)event);
		break;
		/*case QEvent::MouseButtonPress:
			emit mouseClicked((QMouseEvent*)event);
			break;*/
	case QEvent::MouseButtonRelease:
		emit mouseReleased((QMouseEvent*)event);
		break;
	}

	return QWidget::event(event);
}

bool QtRenderWidgetView::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	Q_UNUSED(eventType);
	Q_UNUSED(result);

	if (m_upoSceneManager)
	{
		MSG* pMsg = reinterpret_cast<MSG*>(message);
		NativeWindowProcedure(pMsg);



	}

	return false;

	//return QWidget::nativeEvent(eventType, message, result);
}

LRESULT WINAPI QtRenderWidgetView::NativeWindowProcedure(MSG* pMsg)
{
	return m_upoSceneManager->NativeWindowProcedure(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam);
}

bool QtRenderWidgetView::InitRenderView()
{
	auto poDevice = DxDeviceManager::GetInstance().GetDevice();

	logging::LogAction("");
	logging::LogAction("Make new Device Manager");

	m_upoSceneManager = rldx::DxSceneManager::Create(DxDeviceManager::GetInstance().GetDevice());

	logging::LogAction("Create New Scene");
	
	MakeScene();
	

	// TODO: remove
	//m_upoSceneManager->Resize(rldx::DxDeviceManager::GetInstance().GetDevice(), rldx::DxDeviceManager::GetInstance().GetDeviceContext(), 1024, 1024);

	return true;
}

void QtRenderWidgetView::StartRendering(float framesPerSecond)
{
	m_upoSceneManager->SetRenderRunningState(true);

	// TODO: move this to scene manager?
	m_timer = new QTimer(this);

	connect(m_timer, &QTimer::timeout, [&]()
		{
			FrameTimeOutHandler();
		}
	);

	m_frameTime = 1000.0f / framesPerSecond;
	m_timer->start(m_frameTime);
}

void QtRenderWidgetView::FrameTimeOutHandler()
{
	if (!m_upoSceneManager->IsRenderRunning())
		return;

	m_upoSceneManager->GetCurrentScene()->Draw(rldx::DxDeviceManager::GetInstance().GetDeviceContext());
	m_upoSceneManager->MoveFrame();
}
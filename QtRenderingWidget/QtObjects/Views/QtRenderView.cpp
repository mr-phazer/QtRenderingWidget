#include "QtRenderView.h"

#include "..\..\Rldx\Rldx\Helpers\DxSceneCreator.h"

//#include "..\RenderLib\Direct3d11Device.h"

using namespace rldx;

QtRenderView::QtRenderView(QWidget* parent)
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

void QtRenderView::resizeEvent(QResizeEvent* event)
{
	if (m_upoSceneManager)
	{
		m_timer->stop();
		m_upoSceneManager->Resize(rldx::DxDeviceManager::GetInstance().GetDevice(), rldx::DxDeviceManager::GetInstance().GetDeviceContext(), width(), height());
		m_timer->start();
	}
}
QPaintEngine* QtRenderView::paintEngine() const
{
	return Q_NULLPTR;
}

bool QtRenderView::event(QEvent* event)
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

bool QtRenderView::nativeEvent(const QByteArray& eventType, void* message, long* result)
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

LRESULT WINAPI QtRenderView::NativeWindowProcedure(MSG* pMsg)
{
	return m_upoSceneManager->NativeWindowProcedure(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam);
}

bool QtRenderView::Init(/*rldx::DxDeviceManager* dxManager*/)
{
	auto poDevice = DxDeviceManager::GetInstance().GetDevice();

	logging::LogAction("");
	logging::LogAction("Make new Device Manager");

	m_upoSceneManager = rldx::DxSceneManager::Create(DxDeviceManager::GetInstance().GetDevice());

	logging::LogAction("Create New Scene");
	
	auto newScene =
		rldx::DxNativeWindowSceneBuilder((HWND)winId()).Create(DxDeviceManager::GetInstance().GetDevice(), DxDeviceManager::GetInstance().GetDeviceContext(),	"TestScene1");

	m_upoSceneManager->SetScene(newScene);
	//m_upoSceneManager->Resize(rldx::DxDeviceManager::GetInstance().GetDevice(), rldx::DxDeviceManager::GetInstance().GetDeviceContext(), 1024, 1024);

	return true;
}

void QtRenderView::FrameTimeOutHandler()
{
	if (!m_upoSceneManager->IsRenderRunning())
		return;

	//		renderText(_dxManager);		

	m_upoSceneManager->GetCurrentScene()->Draw(rldx::DxDeviceManager::GetInstance().GetDeviceContext());
	m_upoSceneManager->MoveFrame();
}
#include "QtRenderView.h"

#include <qdir>

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
		ForwardNativeWindowEvents(pMsg);



	}

	return false;

	//return QWidget::nativeEvent(eventType, message, result);
}

LRESULT WINAPI QtRenderWidgetView::ForwardNativeWindowEvents(MSG* pMsg)
{
	return m_upoSceneManager->ForwardNativeWindowEvents(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam);
}

bool QtRenderWidgetView::InitRenderView()
{
	auto poDevice = DxDeviceManager::GetInstance().GetDevice();

	logging::LogAction("");
	logging::LogAction("Make new Device Manager");

	m_upoSceneManager = rldx::DxSceneManager::Create(DxDeviceManager::GetInstance().GetDevice());

	logging::LogAction("Retriving Default Textures from .exe");


	logging::LogAction("Create New Scene");
	
	LoadExeResources(poDevice);

	MakeScene();
	

	// TODO: remove
	//m_upoSceneManager->Resize(rldx::DxDeviceManager::GetInstance().GetDevice(), rldx::DxDeviceManager::GetInstance().GetDeviceContext(), 1024, 1024);

	return true;
}

void QtRenderWidgetView::StartRendering(float framesPerSecond)
{
	m_upoSceneManager->SetRenderRunningState(true);

	if (!m_timer) 
	{
		m_timer = new QTimer(this);

		connect(m_timer, &QTimer::timeout, [&]()
			{
				FrameTimeOutHandler();
			}
		);
	}
	else
	{
		m_timer->stop();
	}

	m_frameTime = 1000.0f / framesPerSecond;
	m_timer->setInterval(1000);
	m_timer->start(m_frameTime);
}

void QtRenderWidgetView::FrameTimeOutHandler()
{
	if (!m_upoSceneManager->IsRenderRunning())
		return;

	m_upoSceneManager->GetCurrentScene()->Draw(rldx::DxDeviceManager::GetInstance().GetDeviceContext());
	m_upoSceneManager->MoveFrame(); // TODO: typically "move geometry" would not be synced with frame-rate like this, but it is good eought for this simple thing.
}

void QtRenderWidgetView::MakeConnections()
{
}

void QtRenderWidgetView::LoadExeResources(ID3D11Device* poDevice)
{
	Q_INIT_RESOURCE(QtRenderView);

	QStringList resourceList = {
		":/QtRenderingWidget/default_base_colour.dds",
		":/QtRenderingWidget/default_blue.dds",
		":/QtRenderingWidget/default_diffuse.dds",
		":/QtRenderingWidget/default_gloss_map.dds",
		":/QtRenderingWidget/default_grey.dds",
		":/QtRenderingWidget/default_metal_material_map.dds",
		":/QtRenderingWidget/default_material_map.dds",
		":/QtRenderingWidget/default_normal.dds",
		":/QtRenderingWidget/default_specular.dds",

		":/Cubemaps/CubeMaps/LandscapeCubeMapIBLDiffuse.dds",
		":/Cubemaps/CubeMaps/LandscapeCubeMapIBLSpecular.dds",
	};

	for (auto& itRes : resourceList)
	{
		QFile file(itRes);
		file.open(QIODevice::ReadOnly);
		auto fileName = QUrl(itRes).fileName();
		auto bytes = file.readAll();

		if (!file.isOpen() || !file.isReadable() || file.size() == 0) {
			throw std::exception((FULL_FUNC_INFO("Error loading internal resource: " + itRes.toStdString())).c_str());
		}

		DxResourceManager::Instance()->AllocTexture(fileName.toStdWString()).GetPtr()->LoadFileFromMemory(poDevice, (uint8_t*)bytes.constData(), bytes.size());
	}
}
void QtRenderWidgetView::focusInEvent(QFocusEvent* event)
{
	// TODO: set frame rate to normal
}

void QtRenderWidgetView::focusOutEvent(QFocusEvent* event)
{
	// TODO: set frame to the lower possible, to allow many views being open without lagging
}
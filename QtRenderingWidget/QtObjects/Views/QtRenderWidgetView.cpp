#include "QtRenderWidgetView.h"

#ifdef DEBUG
// for debugging I need to have access to this, for now
#include "..\..\ExternFunctions\Creators.h" 
#endif // DEBUG

#include <qmessagebox.h>
#include <mouse.h>
#include <qstyle.h>
#include <qstyleoption.h>

#include <Rldx\SceneGraph\SceneNodes\DxMeshNode.h>
#include "..\rldx\rldx\DataTypes\DxMeshData.h"
#include "..\rldx\rldx\Rendering\DxShaderProgram.h"

#include <QMimeData>

#include <qdir>

#include "Logger\Logger.h"

using namespace rldx;
using namespace logging;

QtRenderWidgetView::QtRenderWidgetView(QWidget* parent, const QString& gameidString)
	: QWidget(parent), m_controller(new QtRenderWidgetController(this))
{
	setupUi(this);

#ifdef _DEBUG
	setAcceptDrops(true); // only the debugging version has drag+drop loading
#endif // _DEBUG	

	QPalette pal = palette();
	pal.setColor(QPalette::Window, Qt::black);
	setAutoFillBackground(true);
	setPalette(pal);

	// Qt windows DockWidget has a bug that makes it impossible to set focus to the widget
	setFocusPolicy(Qt::StrongFocus);
	setAttribute(Qt::WA_NativeWindow);

	// Setting these attributes to our widget and returning null on paintEngine event
	// tells Qt that we'll handle all drawing and updating the widget ourselves.
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_DeleteOnClose);

	SetGameIdString(gameidString);
	InitRenderView();

	show();
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

void QtRenderWidgetView::closeEvent(QCloseEvent* event)
{
	/*TerminateRendering();
	emit this->WindowClosing();*/
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
			emit KeyPressed((QKeyEvent*)event);
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
		ForwardNativeWindowEvent(pMsg);



	}

	return false;

	//return QWidget::nativeEvent(eventType, message, result);
}

LRESULT WINAPI QtRenderWidgetView::ForwardNativeWindowEvent(MSG* pMsg)
{
	return m_upoSceneManager->ForwardNativeWindowEvent(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam);
}

bool QtRenderWidgetView::InitRenderView()
{
	logging::LogAction(L"Make new Device Manager");
	DxDeviceManager::Init();
	auto poDevice = DxDeviceManager::GetInstance().GetDevice();

	m_upoSceneManager = rldx::DxSceneManager::CreateScene(DxDeviceManager::GetInstance().GetDevice());

	logging::LogAction(L"Retriving Default Textures from .exe");
	LoadExeResources(poDevice);

	logging::LogAction(L"Create New Scene");
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
					DrawFrameHandler();
				}
		);
	}

	// TODO: remove?
	/*else
	{
		m_timer->stop();
	}*/

	m_frameTime = 1000.0f / framesPerSecond;
	m_timer->setInterval(1000);
	m_timer->start(m_frameTime);
}

void QtRenderWidgetView::TerminateRendering()
{
	PauseRendering();
	// TODO: clear
	//m_upoSceneManager->GetCurrentScene()->Draw
	m_timer->disconnect();
	delete m_timer;
}

void QtRenderWidgetView::DrawFrameHandler()
{
	if (!m_upoSceneManager->IsRenderRunning() || !m_upoSceneManager->GetCurrentScene())
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

		// TODO: remove debuging code
		auto DEBUG__rawPtr = DxResourceManager::Instance()->AllocTexture(fileName.toStdWString(), DxResourceManager::AllocTypeEnum::AttempReuseIdForNew).GetPtr();
		DEBUG__rawPtr->LoadFileFromMemory(poDevice, (uint8_t*)bytes.constData(), bytes.size(), fileName.toStdWString());

		DxResourceManager::Instance()->AllocTexture(fileName.toStdWString(), DxResourceManager::AllocTypeEnum::AttempReuseIdForNew).GetPtr()->LoadFileFromMemory(poDevice, (uint8_t*)bytes.constData(), bytes.size(), fileName.toStdWString());
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

#ifdef _DEBUG
void QtRenderWidgetView::dragEnterEvent(QDragEnterEvent* event)
{
	event->acceptProposedAction();
}

void QtRenderWidgetView::dropEvent(QDropEvent* event)
{
	// Handle drop event (e.g., process dropped file)
	// TODO:: move this code to the CONTROLLER, emit "ItemDropped" and have the controller handle that event with this code
	const QMimeData* mimeData = event->mimeData();
	if (mimeData->hasUrls()) {
		QList<QUrl> urls = mimeData->urls();
		for (const QUrl& url : urls)
		{
			auto filePath = url.toLocalFile().toStdWString();

			utils::ByteStream bytes(filePath);
			QString fileName2 = QString::fromStdWString(filePath);
			QByteArray qBytes2((char*)bytes.GetBufferPtr(), bytes.GetBufferSize());
			QString outErrorString;

			AddNewPrimaryAsset(this, &fileName2, &qBytes2, &outErrorString);

			break; // only read one file, for now
		}
	}
	event->acceptProposedAction();
}
#endif
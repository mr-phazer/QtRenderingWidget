#include "QtRenderView.h"

//#include "..\RenderLib\Direct3d11Device.h"

using namespace rldx;

QtRenderView::QtRenderView(QWidget* parent)
	: QWidget(parent)
{
	setupUi(this);

	/*this->setWindowTitle("QRenderenView (Testing) : Sence");

	setWindowFlag(Qt::Window, true);*/

	show();

	auto debug_1 = 1;
}

bool QtRenderView::Init(/*rldx::DxDeviceManager* dxManager*/)
{
	auto poDevice = DxDeviceManager::GetInstance().GetDevice();

	logging::LogAction("");
	logging::LogAction("Make new Device Manager");

	m_upoSceneManager = rldx::DxSceneManager::Create(DxDeviceManager::GetInstance().GetDevice());

	logging::LogAction("Create New Scene");
	
	auto newScene =
		rldx::NativeWindowSceneCreator((HWND)winId()).Create(DxDeviceManager::GetInstance().GetDevice(), DxDeviceManager::GetInstance().GetDeviceContext(),	"TestScene1");

	m_upoSceneManager->SetScene(newScene);
	m_upoSceneManager->Resize(rldx::DxDeviceManager::GetInstance().GetDevice(), rldx::DxDeviceManager::GetInstance().GetDeviceContext(), 1024, 1024);

	return true;
}

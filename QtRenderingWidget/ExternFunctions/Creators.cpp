#include "Creators.h"

#include <exception>

#include "QtObjects\Views\QtRenderingViewWidget.h"

QWidget* CreateQRenderingWidget(QWidget* parent)
{
	// make sure the Singleton DeviceManager is instantiated
	//auto& spManager = rldx::DxDeviceManager::GetInstance();
	QtRenderingViewWidget* poNewRenderingWidget = nullptr;

	try {
		poNewRenderingWidget = new QtRenderingViewWidget(parent); // nullptr = no parent, free floating window	
		poNewRenderingWidget->hide();
		poNewRenderingWidget->Init();
		
	}
	catch (std::exception& e)
	{
		MessageBoxA(reinterpret_cast<HWND>(parent->winId()), e.what(), "Error: Exception", MB_OK | MB_ICONERROR);
		logging::LogAction(std::string("Error: Excpetion: ") + e.what());
				
		delete poNewRenderingWidget;
		return nullptr;
	}
	
	poNewRenderingWidget->setWindowFlag(Qt::WindowType::Widget);
	poNewRenderingWidget->setWindowTitle("BLAH");// "QRenderingWidget (testing, with various stuff in layout, for testin.");
	poNewRenderingWidget->show();
	poNewRenderingWidget->StartRendering(/*&spManager*/);

	return poNewRenderingWidget;
}

void SetLogPath(const QString& path)
{
	// TODO: implement
	//throw std::exception("The method or operation is not implemented.");
}

void SetAssetPath(const QString& path)
{
	// TODO: implement
	//throw std::exception("The method or operation is not implemented.");
}

#include "Creators.h"

#include <exception>

#include "QtObjects\Views\QtRenderingViewWidget.h"

QWidget* CreateQRenderingWidget(QWidget* parent)
{
	// make sure the Singleton DeviceManager is instantiated
	auto& spManager = rldx::DxDeviceManager::GetInstance();

	QtRenderingViewWidget* poNewRenderingWidget = new QtRenderingViewWidget(parent); // nullptr = no parent, free floating window	

	poNewRenderingWidget->setWindowFlag(Qt::WindowType::Widget);
	poNewRenderingWidget->setWindowTitle("QRenderingWidget (testing, with various stuff in layout, for testin.");
	poNewRenderingWidget->show();

	poNewRenderingWidget->Init(&spManager);
	poNewRenderingWidget->StartRendering(&spManager);

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

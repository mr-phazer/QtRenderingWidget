#include "Creators.h"

#include "QtObjects\Views\QtRenderingViewWidget.h"


QWidget* CreateQRenderingWidget(QWidget* parent)
{
	auto& spManager = Rldx::DxDeviceManager::GetInstance();

	// -- create lots of render view, for demo
	QtRenderingViewWidget* poNewRenderingWidget = new QtRenderingViewWidget(parent); // nullptr = no parent, free floating window	

	poNewRenderingWidget->setWindowFlag(Qt::WindowType::Widget);
	poNewRenderingWidget->setWindowTitle("QRenderingWidget (testing, with various stuff in layout, for testin.");
	poNewRenderingWidget->show();

	poNewRenderingWidget->Init(&spManager);
	poNewRenderingWidget->StartRendering(&spManager);

	return poNewRenderingWidget;
}

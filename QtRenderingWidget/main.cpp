
#include "RenderWindow/View/QtRenderingWidget.h"
#include <QtWidgets/QApplication>

#include "RenderUtil/Managers/DxDeviceManager.h"

int main(int argc, char* argv[]) 
{	
	QApplication a(argc, argv);
	
	// -- create the D3D 11 manager, 
		
	auto spManager = Rldx::DxDeviceManager::GetInstance();

	// -- create lots of render view, for demo
	for (size_t i = 0; i < 4; i++)
	{
		Rldx::QtRenderingWidget* poNewRenderingWidget = new Rldx::QtRenderingWidget(nullptr); // nullptr = no parent, free floating window
		poNewRenderingWidget->Init(spManager);
	}

	return a.exec();
}


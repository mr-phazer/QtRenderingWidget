
#include <string>

#include "QtObjects/Views/QtRenderingWidget.h"
#include <QtWidgets/QApplication>

#include "Rldx/Managers/DxDeviceManager.h"
#include "Rldx/DxObjects/DxShaderProgram.h"
#include "Rldx/Helpers/DxMeshCreator.h"

#include "Logging/Logging.h"

int main(int argc, char* argv[]) 
{	
	QApplication testApplication(argc, argv);	

	
	// -- create the D3D 11 manager, 		
	auto& spManager = Rldx::DxDeviceManager::GetInstance();	
		
	
	// -- create lots of render view, for demo
	QtRenderingWidget* poNewRenderingWidget = new QtRenderingWidget(nullptr); // nullptr = no parent, free floating window
	poNewRenderingWidget->Init(&spManager);

	poNewRenderingWidget->StartRendering(&spManager);	

	return testApplication.exec();
}


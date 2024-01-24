
#include <string>

#include "QtObjects/Views/QtRenderingViewWidget.h"
#include <QtWidgets/QApplication>
#include <QWindow>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlineedit.h>

#include "rldx/Managers/DxDeviceManager.h"
#include "rldx/Rendering/DxShaderProgram.h"
#include "rldx/Helpers/DxMeshCreator.h"

#include "Logging/Logging.h"
#include "ExternFunctions\Creators.h"



int main(int argc, char* argv[]) 
{	
	QApplication testApplication(argc, argv);	


	
	///auto& spManager = rldx::DxDeviceManager::GetInstance();	
	
	auto windows = CreateQRenderingWidget(nullptr);



		

	return testApplication.exec();
}


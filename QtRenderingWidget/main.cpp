
#include <string>

#include "QtObjects/Views/QtRenderView.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QWindow>

#include "..\rldx/rldx/Helpers/DxMeshCreator.h"
#include "..\rldx/rldx/Managers/DxDeviceManager.h"
#include "..\rldx/rldx/Rendering/DxShaderProgram.h"

#include "..\rldx/rldx/Logging/Logging.h"
#include "ExternFunctions\Creators.h"


struct animFile
{

};


int main(int argc, char* argv[])
{
	QApplication testApplication(argc, argv);



	///auto& spManager = rldx::DxDeviceManager::GetInstance();	
	//QString gameId = "wh";
	//auto window = CreateQRenderingWidget(nullptr, &gameId, nullptr);





	return testApplication.exec();
}


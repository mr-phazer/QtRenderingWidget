
#include <string>

#include "QtObjects/Views/QtRenderingWidget.h"
#include <QtWidgets/QApplication>

#include "Rldx/Managers/DxDeviceManager.h"
#include "Rldx/DxObjects/DxShaderProgram.h"

#include "Logging/Logging.h"





int main(int argc, char* argv[]) 
{	
	QApplication a(argc, argv);

	

	// -- create the D3D 11 manager, 
		
	auto& spManager = Rldx::DxDeviceManager::GetInstance();

	int nCorrectName = 1;

	char someName = 1;

	long SomeLong = 1;

	bool bTestFlag = false;

	try 	{
		Rldx::DxMeshShaderProgam testShaderProgram;
		testShaderProgram.Create<Rldx::DxMeshShaderProgam>(
			spManager.GetDevice(),
			LR"(VS_Default.cso)",
			LR"(PS_Simple.cso)");
	}
	catch (const std::exception& e)
	{		
		logging::LogActionError(e.what());
	}
		
	auto resourceByString = Rldx::DxResourceManager::Instance()->GetResourceByString<Rldx::DxMeshShaderProgam>("shader01");

	// -- create lots of render view, for demo
	for (size_t i = 0; i < 4; i++)
	{
		QtRenderingWidget* poNewRenderingWidget = new QtRenderingWidget(nullptr); // nullptr = no parent, free floating window
		poNewRenderingWidget->Init(&spManager);
	}

	return a.exec();
}


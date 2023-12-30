
#include <string>

#include "QtObjects/Views/QtRenderingWidget.h"
#include <QtWidgets/QApplication>
#include <QWindow>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlineedit.h>

#include "Rldx/Managers/DxDeviceManager.h"
#include "Rldx/DxObjects/DxShaderProgram.h"
#include "Rldx/Helpers/DxMeshCreator.h"

#include "Logging/Logging.h"

int main(int argc, char* argv[]) 
{	
	QApplication testApplication(argc, argv);	

	/*auto poContainerWindow = new QFrame (nullptr);
	poContainerWindow->setWindowFlag(Qt::WindowType::Window);
	poContainerWindow->setWindowTitle("QRenderingWidget (testing, with various stuff in layout, for testin.");
	poContainerWindow->show();

	poContainerWindow->setLayout(new QVBoxLayout(poContainerWindow));
	*/
	// -- create the D3D 11 manager, 		
	auto& spManager = Rldx::DxDeviceManager::GetInstance();	
		
	
	// -- create lots of render view, for demo
	QtRenderingWidget* poNewRenderingWidget = new QtRenderingWidget(nullptr); // nullptr = no parent, free floating window	
	
	
	poNewRenderingWidget->setWindowFlag(Qt::WindowType::Window);
	poNewRenderingWidget->setWindowTitle("QRenderingWidget (testing, with various stuff in layout, for testin.");
	poNewRenderingWidget->show();

	//auto poFrame = new QFrame(poContainerWindow);
	//poFrame->setFrameStyle(QFrame::Box | QFrame::Raised);

	//poContainerWindow->layout()->addWidget(poFrame);

	//poFrame->setLayout(new QVBoxLayout(poFrame));

	////poFrame->layout()->addWidget(new QLineEdit("Stuff in Layout", poContainerWindow));
	////poFrame->layout()->addWidget(new QLineEdit("Stuff in Layout", poContainerWindow));
	//poFrame->layout()->addWidget(poNewRenderingWidget);

	poNewRenderingWidget->Init(&spManager);
	poNewRenderingWidget->StartRendering(&spManager);

		

	return testApplication.exec();
}


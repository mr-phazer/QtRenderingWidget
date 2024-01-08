
#include <string>

#include "QtObjects/Views/QtRenderingViewWidget.h"
#include <QtWidgets/QApplication>
#include <QWindow>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlineedit.h>

#include "Rldx/Managers/DxDeviceManager.h"
#include "Rldx/Rendering/DxShaderProgram.h"
#include "Rldx/Helpers/DxMeshCreator.h"

#include "Logging/Logging.h"
#include "ExternFunctions\Creators.h"

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
	
	auto windows = CreateQRenderingWidget(nullptr);

	//
	//// -- create lots of render view, for demo
	//QtRenderingViewWidget* poNewRenderingWidget = new QtRenderingViewWidget(nullptr); // nullptr = no parent, free floating window	
	//
	//
	//poNewRenderingWidget->setWindowFlag(Qt::WindowType::Window);
	//poNewRenderingWidget->setWindowTitle("QRenderingWidget (testing, with various stuff in layout, for testin.");
	//poNewRenderingWidget->show();

	////auto poFrame = new QFrame(poContainerWindow);
	////poFrame->setFrameStyle(QFrame::Box | QFrame::Raised);

	////poContainerWindow->layout()->addWidget(poFrame);

	////poFrame->setLayout(new QVBoxLayout(poFrame));

	//////poFrame->layout()->addWidget(new QLineEdit("Stuff in Layout", poContainerWindow));
	//////poFrame->layout()->addWidget(new QLineEdit("Stuff in Layout", poContainerWindow));
	////poFrame->layout()->addWidget(poNewRenderingWidget);

	//poNewRenderingWidget->Init(&spManager);
	//poNewRenderingWidget->StartRendering(&spManager);

		

	return testApplication.exec();
}


#include "QtRenderingViewWidget.h"

//#include "..\RenderLib\Direct3d11Device.h"

using namespace Rldx;

QtRenderingViewWidget::QtRenderingViewWidget(QWidget* parent)
	: QWidget(parent)
{
	setupUi(this);
	
	/*this->setWindowTitle("QRenderenView (Testing) : Sence");
	
	setWindowFlag(Qt::Window, true);*/

	show();

	auto debug_1 = 1;
}

bool QtRenderingViewWidget::Init(Rldx::DxDeviceManager* dxManager)
{
    

    logging::LogAction("Start Init QRenderingWidget");

    logging::LogAction("Make new Device Manager");
    m_poDxManager = dxManager;
    m_upoSceneManager = Rldx::DxSceneManager::Create(m_poDxManager->GetDevice());

    logging::LogAction("Create New Scene");
    Rldx::NativeWindowSceneCreator sceneCreator(reinterpret_cast<HWND>(this->winId()));
    auto newScene = sceneCreator.Create(m_poDxManager->GetDevice(), "TestScene1");

    logging::LogAction("Loading Shaders");
    auto newPbrShaderProgram =
        Rldx::DxMeshShaderProgram::Create<Rldx::DxMeshShaderProgram>(
            m_poDxManager->GetDevice(),
            tools::GetExePath() + LR"(VS_Simple.cso)",
            tools::GetExePath() + LR"(PS_NoTextures.cso)"
        );

    logging::LogAction("Loading Shaders");
    auto newSimpleShaderProgram =
        Rldx::DxMeshShaderProgram::Create<Rldx::DxMeshShaderProgram>(
            m_poDxManager->GetDevice(),
            tools::GetExePath() + LR"(VS_Simple.cso)",
            tools::GetExePath() + LR"(PS_Simple.cso)"
        );

    auto meshNodeGrid = Rldx::DxMeshNode::Create("Grid");
    auto meshNodeCube = Rldx::DxMeshNode::Create("Cube");

    auto testMeshCube = Rldx::ModelCreator::MakeTestCubeMesh(m_poDxManager->GetDevice());
    meshNodeCube->SetMeshData(testMeshCube);
    meshNodeCube->SetShaderProgram(newPbrShaderProgram);

    auto testMeshGrid = Rldx::ModelCreator::MakeGrid(m_poDxManager->GetDevice(), 20);
    meshNodeGrid->SetMeshData(testMeshGrid);
    meshNodeGrid->SetShaderProgram(newSimpleShaderProgram);

    newScene->GetRootNode()->AddChild(meshNodeGrid);
    newScene->GetRootNode()->AddChild(meshNodeCube);

    m_upoSceneManager->SetScene(newScene);

    m_upoSceneManager->Resize(m_poDxManager->GetDevice(), m_poDxManager->GetDeviceContext(), width(), height());

    return true;
}

#include "QtRenderingViewWidget.h"

//#include "..\RenderLib\Direct3d11Device.h"

using namespace rldx;

QtRenderingViewWidget::QtRenderingViewWidget(QWidget* parent)
	: QWidget(parent)
{
	setupUi(this);
	
	/*this->setWindowTitle("QRenderenView (Testing) : Sence");
	
	setWindowFlag(Qt::Window, true);*/

	show();

	auto debug_1 = 1;
}

bool QtRenderingViewWidget::Init(rldx::DxDeviceManager* dxManager)
{    
    // TODO: cleanup, there is no reason to store the manager.
    m_poDxManager = dxManager;

    logging::LogAction("Start Init QRenderingWidget");

    logging::LogAction("Make new Device Manager");
    
    
    m_upoSceneManager = rldx::DxSceneManager::Create(m_poDxManager->GetDevice());

    logging::LogAction("Create New Scene");
    rldx::NativeWindowSceneCreator sceneCreator(reinterpret_cast<HWND>(this->winId()));
    auto newScene = sceneCreator.Create(m_poDxManager->GetDevice(), "TestScene1");

    /*logging::LogAction("Loading Shaders");
    auto newPbrShaderProgram =
        rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
            m_poDxManager->GetDevice(),
            tools::GetExePath() + LR"(VS_Simple.cso)",
            tools::GetExePath() + LR"(PS_NoTextures.cso)"
        );

    logging::LogAction("Loading Shaders");
    auto newSimpleShaderProgram =
        rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
            m_poDxManager->GetDevice(),
            tools::GetExePath() + LR"(VS_Simple.cso)",
            tools::GetExePath() + LR"(PS_Simple.cso)"
        );

    auto meshNodeGrid = rldx::DxMeshNode::Create("Grid");
    auto meshNodeCube = rldx::DxMeshNode::Create("Cube");

    auto testMeshCube = rldx::ModelCreator::MakeTestCubeMesh(m_poDxManager->GetDevice());
    meshNodeCube->SetMeshData(testMeshCube);
    meshNodeCube->SetShaderProgram(newPbrShaderProgram);

    auto testMeshGrid = rldx::ModelCreator::MakeGrid(m_poDxManager->GetDevice(), 20);
    meshNodeGrid->SetMeshData(testMeshGrid);
    meshNodeGrid->SetShaderProgram(newSimpleShaderProgram);

    newScene->GetRootNode()->AddChild(meshNodeGrid);
    newScene->GetRootNode()->AddChild(meshNodeCube);*/

    m_upoSceneManager->SetScene(newScene);

    m_upoSceneManager->Resize(m_poDxManager->GetDevice(), m_poDxManager->GetDeviceContext(), width(), height());

    return true;
}

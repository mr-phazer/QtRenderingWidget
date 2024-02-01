#include "DxSceneCreator.h"

using namespace rldx;

rldx::DxNativeWindowSceneBuilder::DxNativeWindowSceneBuilder() {}

// TODO: rename 

rldx::DxScene::UniquePtr rldx::DxNativeWindowSceneBuilder::InitNewScene(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::string& name)
{
	 m_newScene = std::make_unique<DxScene>(name);

	SetWindowPos(m_nativeWindowHandle, nullptr, 0, 0, 2000, 1024, SWP_NOOWNERZORDER);
	RECT windowRect;
	GetWindowRect(m_nativeWindowHandle, &windowRect);

	UINT width = windowRect.right - windowRect.left;
	UINT height = windowRect.bottom - windowRect.top;

	// create swap chain
	m_newScene->GetRefSwapChain() = DxSwapChain::CreateForHWND(poDevice, m_nativeWindowHandle, width, height);
	m_newScene->Init(poDevice);

	//newScene->Resize(poDevice, poDeviceContext, 2000, 1024);

	return m_newScene;
}

rldx::DxNativeWindowSceneBuilder::DxNativeWindowSceneBuilder(HWND nativeWindowHandle) : m_nativeWindowHandle(nativeWindowHandle) {}

rldx::DxScene::UniquePtr rldx::DxNativeWindowSceneBuilder::Create(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::string& name)
{
	 m_newScene = InitNewScene(poDevice, poDeviceContext, name);
	///////////////////////////////////////////////////////
	//  START: Make Scene:
	///////////////////////////////////////////////////////			


	auto newSimpleShaderProgram =
		rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
			poDevice,
			libtools::GetExePath() + LR"(VS_Simple.cso)",
			libtools::GetExePath() + LR"(PS_Simple.cso)"
		);

	auto meshNodeGrid = rldx::DxMeshNode::Create("Grid");
	
	auto gridMeshData = rldx::DxMeshCreatorHelper::MakeGrid(poDevice, 20, 0.1f);
	meshNodeGrid->SetMeshData(gridMeshData);
	meshNodeGrid->SetShaderProgram(newSimpleShaderProgram);

	m_newScene->GetRootNode()->AddChild(meshNodeGrid);

	///////////////////////////////////////////////////////
	//  END: Make Scene:
	///////////////////////////////////////////////////////			

	return m_newScene;
}

void rldx::DxNativeWindowSceneBuilder::AddModel(ID3D11Device* poDevice, DxScene* poScene, ByteStream& fileData)
{	
	rmv2::RigidModelReader rigidModelFileReader;
	auto rmv2File = rigidModelFileReader.Read(fileData);
	
	auto modelNodeRmv2 = rldx::DxNodeCreator::CreateNode<DxModelNode>("Model");
	//auto cubMeshData = rldx::DxMeshCreatorHelper::MakeTestCubeMesh(poDevice);			
	//auto rm2MeshData = rldx::DxMeshCreatorHelper::CreateFromRmv2Mesh(poDevice, rmv2File.lods[0].meshBlocks[0]);

	auto newPbrShaderProgram =
	rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
		poDevice,
		libtools::GetExePath() + LR"(VS_Simple.cso)",
		libtools::GetExePath() + LR"(PS_Troy.cso)"
	);

	modelNodeRmv2->SetMeshData(poDevice, rmv2File);
	modelNodeRmv2->SetShaderProgram(newPbrShaderProgram);

	// TODO: debuggin, remove deouble mataterial assignment
	modelNodeRmv2->SetMaterialData(poDevice, rmv2File);
	modelNodeRmv2->SetMaterialData(poDevice, rmv2File);	
	
	poScene->GetRootNode()->AddChild(modelNodeRmv2);
}

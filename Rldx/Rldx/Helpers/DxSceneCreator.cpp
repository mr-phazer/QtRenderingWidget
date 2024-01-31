#include "DxSceneCreator.h"

using namespace rldx;

rldx::DxNativeWindowSceneCreator::DxNativeWindowSceneCreator() {}

// TODO: rename 

rldx::DxScene::UniquePtr rldx::DxNativeWindowSceneCreator::MakeNewScene(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::string& name)
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

rldx::DxNativeWindowSceneCreator::DxNativeWindowSceneCreator(HWND nativeWindowHandle) : m_nativeWindowHandle(nativeWindowHandle) {}

rldx::DxScene::UniquePtr rldx::DxNativeWindowSceneCreator::Create(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::string& name)
{
	 m_newScene = MakeNewScene(poDevice, poDeviceContext, name);
	///////////////////////////////////////////////////////
	//  START: Make Scene:
	///////////////////////////////////////////////////////			


	auto newSimpleShaderProgram =
		rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
			poDevice,
			tools::GetExePath() + LR"(VS_Simple.cso)",
			tools::GetExePath() + LR"(PS_Simple.cso)"
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

void rldx::DxNativeWindowSceneCreator::AddModel(ID3D11Device* poDevice, DxScene* poScene, ByteStream& fileData)
{
	//ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1\emp\emp_karl_franz\emp_karl_franz.rigid_model_v2)");
	//ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1d\def\def_malekith\def_malekith_body_01.rigid_model_v2)");
	rmv2::RigidModelReader rm2Reader;
	auto rmv2File = rm2Reader.Read(fileData);
	
	auto modelNodeRmv2 = rldx::DxNodeCreator::CreateNode<DxModelNode>("Model");
	//auto cubMeshData = rldx::DxMeshCreatorHelper::MakeTestCubeMesh(poDevice);			
	//auto rm2MeshData = rldx::DxMeshCreatorHelper::CreateFromRmv2Mesh(poDevice, rmv2File.lods[0].meshBlocks[0]);

	auto newPbrShaderProgram =
	rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
		poDevice,
		tools::GetExePath() + LR"(VS_Simple.cso)",
		tools::GetExePath() + LR"(PS_Troy.cso)"
	);

	modelNodeRmv2->SetMeshData(poDevice, rmv2File);
	modelNodeRmv2->SetShaderProgram(newPbrShaderProgram);

	// TODO: debuggin, remove deouble mataterial assignment
	modelNodeRmv2->SetMaterialData(poDevice, rmv2File);
	modelNodeRmv2->SetMaterialData(poDevice, rmv2File);	
	
	poScene->GetRootNode()->AddChild(modelNodeRmv2);
}

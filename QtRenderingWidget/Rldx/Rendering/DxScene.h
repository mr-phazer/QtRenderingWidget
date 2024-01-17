#pragma once

#include <memory>
#include <windows.h>

#include <CommonStates.h>

#include "..\..\rldx\Interfaces\IDrawable.h"
#include "..\..\rldx\Rendering\DxShaderProgram.h"
#include "..\Helpers\DxMeshCreator.h"
#include "..\SceneGraph\BaseNode\DxBaseNode.h"
#include "..\SceneGraph\SceneGraph.h"
#include "DxCameraOrbital.h"
#include "DXSwapChain.h"
#include "DxConstBuffer.h"

#include "..\..\ImportExport\RigidModel\Readers\RigidModelReader.h"

namespace rldx {

	enum class DxSceneTypeEnum
	{
		Normal
	};

	class DxScene : public TIdentifiable<DxSceneTypeEnum>, IResizable, IDrawable, IUpdateable
	{
	public:
		
		using UniquePtr = std::unique_ptr<DxScene>;

	public:
		DxScene(const std::string& name = "") : TIdentifiable(name) {};

		virtual void Init(ID3D11Device* poDevice);

		std::string GetTypeString() const override { return "DxScene"; }
		DxSceneTypeEnum GetType() const override { return DxSceneTypeEnum::Normal; }

		virtual void Update(float timeElapsed) override;
		virtual void Draw(ID3D11DeviceContext* poDeviceContext) override;

		DxBaseNode* GetRootNode();
		void DeleteNode(DxBaseNode* node);

		DxSwapChain* GetSwapChain() const
		{
			return m_spoSwapChain.get();
		}

		DxSwapChain::UniquePtr& GetRefSwapChain()
		{
			return m_spoSwapChain;
		}

		LRESULT WINAPI NativeWindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual void Resize(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, unsigned int width, unsigned int height) override;

	private:
		void UpdateViewAndPerspective();
		void UpdateAndBindVSConstBuffer();
		void DEBUGGING_SetViewAndPerspective();

	private:
		DxSceneGraph m_sceneGraph;
		DxMeshRenderBucket m_renderQueue;
		DxSwapChain::UniquePtr m_spoSwapChain; // back buffer	

		std::unique_ptr<DirectX::CommonStates> m_upoCommonStates;

		// TODO: iterate over these, call IResizable::Resize(width, height)
		std::vector<IResizable*> m_resizableObjects; // buffers/etc, that need to be resized when the window is resized

		// TODO: should this be 2 member? think about integrated solution
		TConstBuffer<VS_PerScene_ConstantBuffer> m_sceneFrameVSConstBuffer;
		TConstBuffer<PS_DirectionalLight_ConstBuffer> m_sceneFramePSConstBuffer;

		DxCameraOrbital m_globalCamera;
		DxCameraOrbital m_globalDirectionalLight;
	};

	class IDxSceneCreator
	{
	public:
		virtual DxScene::UniquePtr Create(ID3D11Device* poDevice, const std::string& name = "") = 0;
	};

	class NativeWindowSceneCreator : IDxSceneCreator
	{

		NativeWindowSceneCreator() {};
		// TODO: rename 
		DxScene::UniquePtr MakeNewScene(ID3D11Device* poDevice, const std::string& name = "")
		{
			auto newScene = std::make_unique<DxScene>(name);

			RECT windowRect;
			GetWindowRect(m_nativeWindowHandle, &windowRect);

			UINT width = windowRect.right - windowRect.left;
			UINT height = windowRect.bottom - windowRect.top;

			// create swap chain
			newScene->GetRefSwapChain() = DxSwapChain::CreateForHWND(poDevice, m_nativeWindowHandle, width, height);
			newScene->Init(poDevice);

			return newScene;
		};

	public:
		NativeWindowSceneCreator(HWND nativeWindowHandle) : m_nativeWindowHandle(nativeWindowHandle) {};

		DxScene::UniquePtr Create(ID3D11Device* poDevice, const std::string& name = "") override
		{	
			auto newScene = MakeNewScene(poDevice, name);
			///////////////////////////////////////////////////////
			//  START: Make Scene:
			///////////////////////////////////////////////////////			
			logging::LogAction("Loading Shaders");
			auto newPbrShaderProgram =
				rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
					poDevice,
					tools::GetExePath() + LR"(VS_Simple.cso)",
					tools::GetExePath() + LR"(PS_NoTextures.cso)"
				);

			logging::LogAction("Loading Shaders");
			auto newSimpleShaderProgram =
				rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
					poDevice,
					tools::GetExePath() + LR"(VS_Simple.cso)",
					tools::GetExePath() + LR"(PS_Simple.cso)"
				);

			auto meshNodeGrid = rldx::DxMeshNode::Create("Grid");
			auto meshNodeCube = rldx::DxMeshNode::Create("Cube");


			ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1\emp\emp_karl_franz\emp_karl_franz.rigid_model_v2)");
			rmv2::RigidModelReader reader;
			auto rmv2File = reader.Read(bytes);

			//auto testMeshCube = rldx::ModelCreator::MakeTestCubeMesh(poDevice);
			auto testMeshCube = 
				rldx::ModelCreator::MakeMeshFromRMV2(poDevice, rmv2File.lods[0].meshBlocks[0]);

			meshNodeCube->SetMeshData(testMeshCube);
			meshNodeCube->SetShaderProgram(newPbrShaderProgram);

			auto testMeshGrid = rldx::ModelCreator::MakeGrid(poDevice, 20, 0.1);
			meshNodeGrid->SetMeshData(testMeshGrid);
			meshNodeGrid->SetShaderProgram(newSimpleShaderProgram);

			newScene->GetRootNode()->AddChild(meshNodeGrid);
			newScene->GetRootNode()->AddChild(meshNodeCube);
			///////////////////////////////////////////////////////
			//  END: Make Scene:
			///////////////////////////////////////////////////////			
			
			return newScene;
		};

	private:
		HWND m_nativeWindowHandle = static_cast<HWND>(0);
	};
};
#pragma once

#include <memory>
#include <windows.h>

#include <CommonStates.h>

#include "..\..\rldx\Interfaces\IDrawable.h"
#include "..\..\rldx\Rendering\DxShaderProgram.h"
#include "..\Helpers\DxMeshCreator.h"
#include "..\Managers\DxDeviceManager.h"
#include "..\SceneGraph\SceneGraph.h"
#include "..\SceneGraph\SceneNodes\DxBaseNode.h"
#include "..\SceneGraph\SceneNodes\DxModelNode.h"
#include "DxAmbientLightSource.h"
#include "DxCameraOrbital.h"
#include "DxConstBuffer.h"
#include "DXSwapChain.h"
#include "DxTextureSamplers.h"

#include "..\DataTypes\ConstBuffers\CPUConstBuffers.h"

#include "..\..\ImportExport\RigidModel\Readers\RigidModelReader.h"

namespace rldx {

	enum PS_ConstBufferSlots : UINT
	{
		AmbientLight = 0,
		Lights = 1
	};

	enum class DxSceneTypeEnum
	{
		Normal
	};

	class DxScene : public TIdentifiable<DxSceneTypeEnum>, IResizable, IDrawable, IUpdateable, IBindable
	{
	public:
		friend class IDxSceneCreator;

		using UniquePtr = std::shared_ptr<DxScene>;

	public:
		DxScene(const std::string& name = "") : TIdentifiable(name) {};
		virtual void Init(ID3D11Device* poDevice);

		std::string GetTypeString() const override { return "DxScene"; }
		DxSceneTypeEnum GetType() const override { return DxSceneTypeEnum::Normal; }

		virtual void Update(float timeElapsed) override;
		virtual void Draw(ID3D11DeviceContext* poDeviceContext) override;

		DxBaseNode* GetRootNode();
		void DeleteNode(DxBaseNode* node);

		const DxSwapChain* GetSwapChain() const { return m_spoSwapChain.get(); }
		DxSwapChain::UniquePtr& GetRefSwapChain() { return m_spoSwapChain; }

		void MakeSceneSwapChain(ID3D11Device* poDevice, HWND nativeWindowHandle);
		virtual void Resize(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, unsigned int width, unsigned int height) override;

		LRESULT WINAPI NativeWindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual void BindToDC(ID3D11DeviceContext* poDC) override;

	private:
		void UpdateViewAndPerspective();


		//void UpdateAmbientLight();


		void DEBUGGING_SetViewAndPerspective();

		void BindAllBindable(ID3D11DeviceContext* poDeviceContext)
		{
			for (auto& bindable : m_bindableObjects)
			{
				bindable->BindToDC(poDeviceContext);
			}
		}

	private:
		DxSceneGraph m_sceneGraph;
		DxMeshRenderBucket m_renderQueue;
		DxSwapChain::UniquePtr m_spoSwapChain; // back buffer	

		std::unique_ptr<DirectX::CommonStates> m_upoCommonStates;

		// TODO: iterate over these, call IResizable::Resize(width, height)
		std::vector<IResizable*> m_resizableObjects; // buffers/etc, that need to be resized when the window is resized
		std::vector<IBindable*> m_bindableObjects; // buffers/etc, that need to be resized when the window is resized

		DxCameraOrbital m_globalCamera;
		DxCameraOrbital m_globalDirectionalLight;

		// -- Bindable objects
		// TODO: should this be 2 member? think about integrated solution
		TDxVSShaderConstBuffer<VS_PerScene_ConstantBuffer> m_sceneFrameVSConstBuffer;
		TDxPSShaderConstBuffer<PS_Light_ConstBuffer> m_sceneFramePSConstBuffer;
				
		DxAmbientLightSource m_ambientLightSource;
		DxTextureSamplers m_textureSamplers;

	private:
		HWND m_hwndNativeWindowHandle = static_cast<HWND>(0);
	};


	class IDxSceneCreator
	{
	public:
		virtual DxScene::UniquePtr Create(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::string& name = "") = 0;
		
	};

	class NativeWindowSceneCreator : IDxSceneCreator
	{

		NativeWindowSceneCreator() {};
		// TODO: rename 
		DxScene::UniquePtr MakeNewScene(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::string& name = "")
		{

			auto newScene = std::make_unique<DxScene>(name);

			SetWindowPos(m_nativeWindowHandle, nullptr, 0, 0, 2000, 1024, SWP_NOOWNERZORDER);
			RECT windowRect;
			GetWindowRect(m_nativeWindowHandle, &windowRect);

			UINT width = windowRect.right - windowRect.left;
			UINT height = windowRect.bottom - windowRect.top;

			// create swap chain
			newScene->GetRefSwapChain() = DxSwapChain::CreateForHWND(poDevice, m_nativeWindowHandle, width, height);
			newScene->Init(poDevice);

			//newScene->Resize(poDevice, poDeviceContext, 2000, 1024);

			return newScene;
		};

	public:
		NativeWindowSceneCreator(HWND nativeWindowHandle) : m_nativeWindowHandle(nativeWindowHandle) {};

		DxScene::UniquePtr Create(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::string& name = "") override
		{		auto newScene = MakeNewScene(poDevice, poDeviceContext, name);
				///////////////////////////////////////////////////////
				//  START: Make Scene:
				///////////////////////////////////////////////////////			
						
				auto newPbrShaderProgram =
					rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
						poDevice,
						tools::GetExePath() + LR"(VS_Simple.cso)",
						tools::GetExePath() + LR"(PS_Troy.cso)"
					);
		
				auto newSimpleShaderProgram =
					rldx::DxMeshShaderProgram::Create<rldx::DxMeshShaderProgram>(
						poDevice,
						tools::GetExePath() + LR"(VS_Simple.cso)",
						tools::GetExePath() + LR"(PS_Simple.cso)"
					);

				auto meshNodeGrid = rldx::DxMeshNode::Create("Grid");
				auto modelNodeRmv2 = rldx::DxNodeCreator::CreateNode<DxModelNode>("Model");

				//ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1\emp\emp_karl_franz\emp_karl_franz.rigid_model_v2)");
				ByteStream bytes(LR"(K:\Modding\WH2\variantmeshes\wh_variantmodels\hu1d\def\def_malekith\def_malekith_body_01.rigid_model_v2)");
				rmv2::RigidModelReader rm2Reader;
				auto rmv2File = rm2Reader.Read(bytes);

				//auto cubMeshData = rldx::DxMeshCreatorHelper::MakeTestCubeMesh(poDevice);			
				//auto rm2MeshData = rldx::DxMeshCreatorHelper::CreateFromRmv2Mesh(poDevice, rmv2File.lods[0].meshBlocks[0]);

				modelNodeRmv2->SetMeshData(poDevice, rmv2File);
				modelNodeRmv2->SetShaderProgram(newPbrShaderProgram);
				
				// TODO: debuggin, remove deouble mataterial assignment
				modelNodeRmv2->SetMaterialData(poDevice, rmv2File);
				modelNodeRmv2->SetMaterialData(poDevice, rmv2File);
				

				auto gridMeshData = rldx::DxMeshCreatorHelper::MakeGrid(poDevice, 20, 0.1f);
				meshNodeGrid->SetMeshData(gridMeshData);
				meshNodeGrid->SetShaderProgram(newSimpleShaderProgram);

				newScene->GetRootNode()->AddChild(meshNodeGrid);
				newScene->GetRootNode()->AddChild(modelNodeRmv2);


				///////////////////////////////////////////////////////
				//  END: Make Scene:
				///////////////////////////////////////////////////////			

				return newScene;

			

		};

	private:
		HWND m_nativeWindowHandle = static_cast<HWND>(0);
	};
};
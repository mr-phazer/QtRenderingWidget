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
		sTIFF= 0,
		Lights = 1
	};

	enum class DxSceneTypeEnum
	{
		Normal
	};

	class DxScene : public TIdentifiable<DxSceneTypeEnum>, IResizable, IDrawable, IUpdateable, IBindable
	{
	public:
		friend class IDxSceneBuilder;

		using UniquePtr = std::shared_ptr<DxScene>;

	public:
		DxScene(const std::string& name = "") : TIdentifiable(name) 
		{
			DxDeviceManager::GetInstance().GetDebugTextWriter()->AddString(L"QtRenderingViewWidget for RPFM version 0.0.1a.");
		};

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


};
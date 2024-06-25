#pragma once

#include <memory>
#include <windows.h>

#include <CommonStates.h>

#include "..\..\rldx\Interfaces\IDrawable.h"
#include "..\..\rldx\Rendering\DxShaderProgram.h"
#include "..\Creators\DxMeshCreator.h"
#include "..\Managers\DxDeviceManager.h"
#include "..\Managers\VmdManager\DxVmdManager.h"
#include "..\SceneGraph\SceneGraph.h"
#include "..\SceneGraph\SceneNodes\DxBaseNode.h"
#include "..\SceneGraph\SceneNodes\DxModelNode.h"
#include "DxAmbientLightSource.h"
#include "DxCameraOrbital.h"
#include "DxConstBuffer.h"
#include "DxMeshRenderBucket.h"
#include "DXSwapChain.h"
#include "DxTextureSamplers.h"

#include "..\DataTypes\ConstBuffers\CPUConstBuffers.h"

#include "..\..\ImportExport\FileFormats\RigidModel\Readers\RigidModelReader.h"

// # forward decl
namespace rldx {
	class DxMeshRenderBucket;
}
namespace rldx {

	enum PS_ConstBufferSlots : UINT
	{
		sTIFF = 0,
		Lights = 1
	};

	enum class DxSceneTypeEnum
	{
		Normal
	};

	class DxScene : public TIdentifiable<DxSceneTypeEnum>, public IResizable, public IDrawable, public IUpdateable, public IBindable
	{
		friend class DxSceneGraph;

	public:
		friend class IDxSceneBuilder;
		friend class DxSceneCreator;

		using Uptr = std::unique_ptr<DxScene>;

	public:
		DxScene() = default;
		DxScene(const std::wstring& name = L"") : TIdentifiable(name)
		{
			DxDeviceManager::GetInstance().GetDebugTextWriter()->AddString(L"QtRenderingWidget for RPFM version 0.0.1a.", { 1,1,1,1 }, 6.0f);
		};
		DxVmdManager& GetVmdManager() { return m_vmdManager; }

		std::wstring GetTypeString() const override { return L"DxScene"; }
		DxSceneTypeEnum GetType() const override { return DxSceneTypeEnum::Normal; }

		virtual void InitRenderView(ID3D11Device* poDevice);
		virtual void Update(float timeElapsed) override;
		virtual void Draw(ID3D11DeviceContext* poDeviceContext) override;
		virtual void Resize(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, unsigned int width, unsigned int height) override;
		virtual void BindToDC(ID3D11DeviceContext* poDC) override;

		DxBaseNode* GetSceneRootNode() const;
		DirectX::BoundingBox GetRootBoundBox() { return m_sceneGraph.GetRootBoundBox(); }

		void SetGridState(DxBaseNode::DrawStateEnum drawState);
		DxMeshNode* GetGridNode() const;

		void DeleteNode(DxBaseNode* node);

		const DxSwapChain* GetSwapChain() const { return m_spoSwapChain.get(); }
		DxSwapChain::Uptr& GetRefSwapChain() { return m_spoSwapChain; }

		LRESULT WINAPI ForwardNativeWindowEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// TODO: remove?
		void SetDefaultShaderProgram(DxMeshShaderProgram* shaderProgram)
		{
			m_poDefaultShaderProgram = shaderProgram;
		};

		// TODO: remove?
		DxMeshShaderProgram* GeDefaultShaderProgram() const
		{
			return m_poDefaultShaderProgram;
		}

		DxCameraOrbital& GetCamera() { return m_globalCamera; };
		DxSceneGraph& GetSceneGraph() { return m_sceneGraph; };

	private:
		void StoreNode(DxBaseNode* node);

		void UpdateViewAndPerspective();

		// TODO: remove?
		//void UpdateAmbientLight();

		// TODO: remove?
		void DEBUGGING_SetViewAndPerspective();

		// TODO: remove?
		//void BindAllBindable(ID3D11DeviceContext* poDeviceContext)
		//{
		//	for (auto& bindable : m_bindableObjects)
		//	{
		//		bindable->BindToDC(poDeviceContext);
		//	}
		//}



	private:
		DxVmdManager m_vmdManager;
		DxSceneGraph m_sceneGraph;
		DxMeshRenderBucket m_renderQueue;
		DxSwapChain::Uptr m_spoSwapChain; // back buffer	

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
		DxMeshShaderProgram* m_poDefaultShaderProgram = nullptr;
		DxMeshNode* m_poGridNode = nullptr;

	private:
		HWND m_hwndNativeWindowHandle = static_cast<HWND>(0);
		bool m_bCtrlDown = false; // TODO: should not be here, a more formal "input handler" is needed
		bool m_bAltDown = false; // TODO: should not be here, a more formal "input handler" is needed
	};
};
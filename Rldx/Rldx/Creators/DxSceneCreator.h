#pragma once

#include <ImportExport\Helpers\Templates.h>
#include <Rldx\Rldx\Managers\VmdManager\DxVmdManager.h>
#include <Rldx\Rldx\Rendering\DxScene.h>

namespace rldx {

	enum RGBModeEnum : bool
	{
		SRGB_Mode = true,
		RGB_Mode = false
	};

	class IDxSceneBuilder
	{
	public:
		virtual std::unique_ptr<DxScene> Create(HWND, ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::wstring& gameIdStromg) = 0;

	};

	class DxSceneCreator : public IDxSceneBuilder
	{
		HWND m_nativeWindowHandle = static_cast<HWND>(NULL);
		std::unique_ptr<DxScene> m_upoNewScene;

	private:
		std::unique_ptr<DxScene> InitSceneDX(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, bool isSRGB = true, const std::wstring& m_nodeName = L"");;
		static void SetCameraAutoFit(rldx::DxScene* poScene);
		void AddGrid(ID3D11Device* poDevice, rldx::DxMeshShaderProgram* newSimpleShaderProgram);

	public:
		std::unique_ptr<DxScene> Create(HWND nativeWindHande, ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::wstring& gameIdString) override;
		static void AddVariantMesh(ID3D11Device* poDevice, DxScene* poScene, utils::ByteStream& fileData, const std::wstring& gameIdString);
		static void AddModel(ID3D11Device* poDevice, DxScene* poScene, utils::ByteStream& fileData, const std::wstring& gameIdString);
	};
}
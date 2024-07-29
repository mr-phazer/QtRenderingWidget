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
		virtual std::unique_ptr<DxScene> Create(HWND nativeWindHande, ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, rldx::DxResourceManager& resourcemanager, const std::wstring& gameIdString) = 0;
	};

	class DxSceneCreator : public IDxSceneBuilder
	{
		HWND m_nativeWindowHandle = static_cast<HWND>(NULL);
		std::unique_ptr<rldx::DxScene> m_upoNewScene;

	private:
		std::unique_ptr<DxScene> CreateScene(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, rldx::DxResourceManager& resourceManager, bool isSRGB = true, const std::wstring& m_nodeName = L"");;
		static void SetCameraAutoFit(DxScene* poScene);
		void AddGrid(DxResourceManager& resourceManager, ID3D11Device* poDevice);

	public:
		std::unique_ptr<DxScene> Create(HWND nativeWindHande, ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, rldx::DxResourceManager& resourcemanager, const std::wstring& gameIdString) override;
		static void AddVariantMesh(ID3D11Device* poDevice, rldx::DxResourceManager& resourceManager, DxScene* poScene, utils::ByteStream& fileData, const std::wstring& gameIdString);
		static void CreateNewGrid(DxResourceManager& resourcemanager, ID3D11Device* poDevice, rldx::DxScene* poScene);
		//static void AddModel(ID3D11Device* poDevice, DxScene* poScene, utils::ByteStream& fileData, const std::wstring& gameIdString);
	};
}
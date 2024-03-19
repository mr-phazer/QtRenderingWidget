#pragma once

#include "..\..\ImportExport\Helpers\Templates.h"
#include "..\Managers\VmdManager\DxVmdManager.h"
#include "..\Rendering\DxScene.h"

namespace rldx {

	enum RGBModeEnum : bool
	{
		SRGB_Mode = true,
		RGB_Mode = false
	};

	class IDxSceneBuilder
	{
	public:
		virtual DxScene::Uptr Create(HWND, ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::wstring& gameIdStromg) = 0;

	};

	class DxSceneCreator : public IDxSceneBuilder
	{
		HWND m_nativeWindowHandle = static_cast<HWND>(0);
		DxScene::Uptr m_newScene;

	private:
		DxScene::Uptr InitNewScene(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, bool isSRGB = true, const std::wstring& name = L"");;
		static void SetCameraAutoFit(rldx::DxScene* poScene);
		void AddGrid(ID3D11Device* poDevice, rldx::DxMeshShaderProgram* newSimpleShaderProgram);

	public:
		DxScene::Uptr Create(HWND nativeWindHande, ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::wstring& gameIdString) override;
		static void AddVariantMesh(ID3D11Device* poDevice, DxScene* poScene, ByteStream& fileData, const std::wstring& gameIdString);
		static void AddModel(ID3D11Device* poDevice, DxScene* poScene, ByteStream& fileData, const std::wstring& gameIdString);
	};
}
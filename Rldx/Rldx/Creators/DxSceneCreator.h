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

		DxScene::Uptr InitNewScene(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, bool isSRGB = true, const std::wstring& name = L"");;

	public:
		DxScene::Uptr Create(HWND nativeWindHande, ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::wstring& gameIdString) override;

		static void TESTCODE_AddVMD(ID3D11Device* poDevice, DxScene* poScene, ByteStream& fileData, const std::wstring& gameIdString);

		static void AddModel(ID3D11Device* poDevice, DxScene* poScene, ByteStream& fileData, const std::wstring& gameIdString);

	private:
		static void SetCameraAutoFit(DxModelNode* modelNodeRmv2, rldx::DxScene* poScene);

	private:
		HWND m_nativeWindowHandle = static_cast<HWND>(0);
		DxScene::Uptr m_newScene;
	};


	//class SceneCreatorFactory : public templates::TAbstractFactory<IDxSceneBuilder, std::string>
	//{
	//	
	//};
}
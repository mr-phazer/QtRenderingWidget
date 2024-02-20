#pragma once

#include "..\Rendering\DxScene.h"
#include "..\Managers\VmdManager\DxVmdManager.h"
#include "..\..\ImportExport\Helpers\Templates.h"

namespace rldx {

	enum RGBModeEnum : bool
	{
		SRGB_Mode = true,
		RGB_Mode = false
	};

	class IDxSceneBuilder
	{
	public:
		virtual DxScene::Uptr Create(HWND, ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::string& name = "") = 0;

	};

	class DxSceneCreator : public IDxSceneBuilder
	{

		DxVmdManager m_variantMeshCreator;

		DxScene::Uptr InitNewScene(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, bool isSRGB = true, const std::string& name = "");;		

	public:
		DxScene::Uptr Create(HWND nativeWindHande, ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::string& gameIdString) override;	
		static void AddModel(ID3D11Device* poDevice, DxScene* poScene, ByteStream& fileData, const std::string& gameIdString);

	private:
		static void SetCameraAutoFit(std::shared_ptr<rldx::DxModelNode>& modelNodeRmv2, rldx::DxScene* poScene);

	private:
		HWND m_nativeWindowHandle = static_cast<HWND>(0);
		DxScene::Uptr m_newScene;
	};


	//class SceneCreatorFactory : public templates::TAbstractFactory<IDxSceneBuilder, std::string>
	//{
	//	
	//};
}
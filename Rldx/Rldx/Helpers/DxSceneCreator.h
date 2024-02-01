#pragma once

#include "..\Rendering\DxScene.h"

namespace rldx {

	class IDxSceneBuilder
	{
	public:
		virtual DxScene::UniquePtr Create(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::string& name = "") = 0;

	};

	class DxNativeWindowSceneBuilder : IDxSceneBuilder
	{

		DxScene::UniquePtr InitNewScene(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::string& name = "");;
		DxNativeWindowSceneBuilder();// TODO: rename 		

	public:
		DxNativeWindowSceneBuilder(HWND nativeWindowHandle);;

		DxScene::UniquePtr Create(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::string& name = "") override;
		static void AddModel(ID3D11Device* poDevice, DxScene* scene, ByteStream& data);

	private:
		HWND m_nativeWindowHandle = static_cast<HWND>(0);
		DxScene::UniquePtr m_newScene;
	};

}
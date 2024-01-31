#pragma once

#include "..\Rendering\DxScene.h"

namespace rldx {

	class IDxSceneCreator
	{
	public:
		virtual DxScene::UniquePtr Create(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::string& name = "") = 0;

	};

	class DxNativeWindowSceneCreator : IDxSceneCreator
	{

		DxNativeWindowSceneCreator();
		// TODO: rename 
		DxScene::UniquePtr MakeNewScene(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::string& name = "");;

	public:
		DxNativeWindowSceneCreator(HWND nativeWindowHandle);;

		DxScene::UniquePtr Create(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, const std::string& name = "") override;
		static void AddModel(ID3D11Device* poDevice, DxScene* scene, ByteStream& data);

	private:
		HWND m_nativeWindowHandle = static_cast<HWND>(0);
		DxScene::UniquePtr m_newScene;
	};

}
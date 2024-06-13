
#include <windows.h>
#include "..\..\..\DirectXTK\Src\PlatformHelpers.h"
#include "Logger\Logger.h"
#include "..\Tools\tools.h"
#include "DxDeviceManager.h"



using namespace rldx;
using namespace logger;

std::unique_ptr<DxDeviceManager> DxDeviceManager::sm_spoInstance = nullptr;

DxDeviceManager::PUnique rldx::DxDeviceManager::Create()
{
	Logger::LogAction("Creating New Device Mangager");
	auto poNewInstance = std::unique_ptr<DxDeviceManager>(new DxDeviceManager);

	Logger::LogAction("Calling InitDirectd3d11..");
	HRESULT hr = poNewInstance->InitDirect3d11();

	if (!SUCCEEDED(hr)) {
		Logger::LogActionError(L"Init d3d 11 failed!");
		throw DirectX::com_exception(hr);
	}

	Logger::LogAction("Create State Object..");
	// Create state objects.
	poNewInstance->m_stateObjects = std::make_unique<DirectX::CommonStates>(poNewInstance->GetDevice());

	Logger::LogAction("Create Debug Text Writer..");
	poNewInstance->m_upoDebugTextWriter = DxDebugTextWriter::Create(poNewInstance->GetDevice(), poNewInstance->GetDeviceContext());

	return poNewInstance;
}

DxDeviceManager& rldx::DxDeviceManager::GetInstance()
{
	if (!sm_spoInstance) {
		sm_spoInstance = Create();
	}
	return *sm_spoInstance.get();
}

HRESULT rldx::DxDeviceManager::InitDirect3d11()
{
	Logger::LogAction("Attempting to create Direct3d 11 Device...");

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		// TODO: check that this will work with the shader model 5.0
		D3D_FEATURE_LEVEL_11_0, // fallback for older gpu's.

		//D3D_FEATURE_LEVEL_10_1,
		//D3D_FEATURE_LEVEL_10_0,
		//D3D_FEATURE_LEVEL_9_3,
		//D3D_FEATURE_LEVEL_9_2,
		//D3D_FEATURE_LEVEL_9_1,
	};

	HRESULT hr = S_OK;
	try
	{

#ifdef NOT_DEBUG
		hr = D3D11CreateDevice(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			D3D11_CREATE_DEVICE_DEBUG,
			featureLevels,
			_countof(featureLevels),
			D3D11_SDK_VERSION,
			&m_cpoDevice,
			NULL,
			&m_cpoDeviceContext
		);
#else
		HRESULT hr = D3D11CreateDevice(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			0,
			featureLevels,
			_countof(featureLevels),
			D3D11_SDK_VERSION,
			&m_cpoDevice,
			NULL,
			&m_cpoDeviceContext);
#endif
	}
	catch (libtools::COMException& comException)
	{
		_com_error err(comException);
		LPCTSTR errMsg = err.ErrorMessage();
		Logger::LogActionError(L"Direct3d 11 Device failed! HRESULT code: " + ToWString(to_string(hr)) + L": " + errMsg);

		MessageBox(nullptr, errMsg, L"Come Error", MB_OK);
	}

	if (!SUCCEEDED(hr))
	{
		_com_error err(hr);
		LPCTSTR errMsg = err.ErrorMessage();
		Logger::LogActionError(L"Direct3d 11 Device failed! HRESULT code: " + ToWString(to_string(hr)) + L": " + errMsg);

		MessageBox(nullptr, errMsg, L"Come Error", MB_OK);
	}

	Logger::LogActionSuccess(L"D3D11CreateDevice()...Finsihed");

	return hr;
}

bool rldx::DxDeviceManager::InitFont()
{
	// TODO: exception?
	/*fontEngine.m_upoFont = std::make_unique<DirectX::SpriteFont>(GetDevice(), L"myfile.spritefont");
	fontEngine.m_upoSpriteBatch = std::make_unique<DirectX::SpriteBatch>(GetDeviceContext());

	if (!(fontEngine.m_upoFont && fontEngine.m_upoSpriteBatch))
	{
		throw exception("Error Loading Font");
	}*/

	return true;
}

void rldx::DxDeviceManager::RenderText()
{

	//spSwapChain->getBackBuffer().clearPixels(_dxManager->deviceContext(), spSwapChain->Color);
	//_dxManager->deviceContext()->OMSetRenderTargets(1, spSwapChain->getBackBuffer().getAddressOfRenderTargetView(), nullptr);





	DirectX::SimpleMath::Vector2 origin = { 0,0 }; // m_upoFont->MeasureString(output) / 2.f;

	DirectX::SimpleMath::Vector3 vScale = { 5.f, 5.f, 5.f };
	DirectX::SimpleMath::Vector2 vPosition = { 0.f, 0.f };
	DirectX::SimpleMath::Color vColor = { 0.0f, 1.0f, 1.0f, 0.0f };
	float rotation = 0.0f;

	// Create an alpha enabled blend state description.
	//const float blendFactor[4] = {1,1,1,1};
	//m_poScene->getDeviceContext()->OMSetBlendState(poBlendState, blendFactor, 0xFFFFFFFF);

	//GetDeviceContext()->OMSetDepthStencilState(depthStencilStates.Off.Get(), 1);


	//fontEngine.m_upoSpriteBatch->Begin();
	//for (auto& itStr : fontEngine.m_stringRenderQueue)
	//{
	//	auto boundRect = fontEngine.m_upoFont->MeasureDrawBounds(itStr.c_str(), vPosition, false);

	//	fontEngine.m_upoFont->DrawString(
	//		fontEngine.m_upoSpriteBatch.get(),
	//		itStr.c_str(),
	//		vPosition,
	//		vColor,
	//		rotation,
	//		origin,
	//		vScale,
	//		DirectX::SpriteEffects::SpriteEffects_None
	//	);

	//	vPosition.y += boundRect.bottom;
	//}
	//fontEngine.m_upoSpriteBatch->End();
}

#include "DxSceneManager.h"
#include "..\DxObjects\DXSwapChain.h"
#include "DxDeviceManager.h"
#include "..\..\Logging\Logging.h"
#include "..\..\Tools\tools.h"

using namespace Rldx;

std::unique_ptr<DxDeviceManager> DxDeviceManager::sm_spoInstance = nullptr;

DxDeviceManager::UPtr Rldx::DxDeviceManager::Create()
{
	auto poNewInstance = std::make_unique<DxDeviceManager>();

	HRESULT hr = poNewInstance->InitDirect3d11();

	assert(SUCCEEDED(hr));

	return poNewInstance;
}

HRESULT Rldx::DxDeviceManager::InitDirect3d11()
{
	logging::LogAction("Attempting to create Direct3d 11 Device...");	
	
	D3D_FEATURE_LEVEL featureLevels[] = {		
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		//D3D_FEATURE_LEVEL_10_1,
		//D3D_FEATURE_LEVEL_10_0,
		//D3D_FEATURE_LEVEL_9_3,
		//D3D_FEATURE_LEVEL_9_2,
		//D3D_FEATURE_LEVEL_9_1,
	};

#ifdef _DEBUG
	HRESULT hr = S_OK;
	try
	{
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
	assert(SUCCEEDED(hr));;
#endif

	}
	catch (tools::COMException& comException)
	{
		_com_error err(comException);
		LPCTSTR errMsg = err.ErrorMessage();
		string strErrorMessage = tools::wstring_to_string(errMsg);
		logging::LogActionError("Direct3d 11 Device failed! HRESULT code: " + to_string(hr) + ": " + strErrorMessage);
		
		MessageBox(nullptr, errMsg, L"Come Error", MB_OK);
	}

	if (!SUCCEEDED(hr))
	{
		_com_error err(hr);
		LPCTSTR errMsg = err.ErrorMessage();
		string strErrorMessage = tools::wstring_to_string(errMsg);
		logging::LogActionError("Direct3d 11 Device failed! HRESULT code: " + to_string(hr) + ": " + strErrorMessage);

		MessageBox(nullptr, errMsg, L"Come Error", MB_OK);
	}

	logging::LogActionSuccess("D3D11CreateDevice()...Finsihed");

	return hr;
}

bool Rldx::DxDeviceManager::InitFont()
{
	// TODO: exception?
	fontEngine.m_font = std::make_unique<DirectX::SpriteFont>(GetDevice(), L"myfile.spritefont");
	fontEngine.m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(GetDeviceContext());

	if (!(fontEngine.m_font && fontEngine.m_spriteBatch))
	{
		throw exception("Error Loading Font");
	}
}

void Rldx::DxDeviceManager::RenderText()
{

	//spSwapChain->getBackBuffer().clearPixels(_dxManager->deviceContext(), spSwapChain->Color);
	//_dxManager->deviceContext()->OMSetRenderTargets(1, spSwapChain->getBackBuffer().getAddressOfRenderTargetView(), nullptr);





	DirectX::SimpleMath::Vector2 origin = { 0,0 }; // m_font->MeasureString(output) / 2.f;

	DirectX::SimpleMath::Vector3 vScale = { 5.f, 5.f, 5.f };
	DirectX::SimpleMath::Vector2 vPosition = { 0.f, 0.f };
	DirectX::SimpleMath::Color vColor = { 0.0f, 1.0f, 1.0f, 0.0f };
	float rotation = 0.0f;

	// Create an alpha enabled blend state description.
	//const float blendFactor[4] = {1,1,1,1};
	//m_poScene->getDeviceContext()->OMSetBlendState(poBlendState, blendFactor, 0xFFFFFFFF);

	GetDeviceContext()->OMSetDepthStencilState(depthStencilStates.Off.Get(), 1);


	fontEngine.m_spriteBatch->Begin();
	for (auto& itStr : fontEngine.m_stringsToDraw)
	{
		auto boundRect = fontEngine.m_font->MeasureDrawBounds(itStr.c_str(), vPosition, false);

		fontEngine.m_font->DrawString(
			fontEngine.m_spriteBatch.get(),
			itStr.c_str(),
			vPosition,
			vColor,
			rotation,
			origin,
			vScale,
			DirectX::SpriteEffects::SpriteEffects_None
		);

		vPosition.y += boundRect.bottom;
	}
	fontEngine.m_spriteBatch->End();
}

#pragma once

#include <d3d11.h>
#include <d3d11_1.h>
#include <D3Dcompiler.h>

#include <assert.h>

//  WRL wrappers from COM Objects - smart pointers for Direct3d resources
#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>

// Standard lib headers
#include <stdexcept>
#include <memory>
#include <functional>
#include <vector>
#include <memory>

#pragma comment(lib, "d3d11.lib")

#include "..\DxObjects\DXSwapChain.h"
#include "..\..\..\DirectXTK\Inc\SpriteFont.h"
#include "..\..\Logging\Logging.h"


#include <iostream>



namespace Rldx
{

	/// <summary>
	/// singleton only 1 device per .exe
	/// </summary>
	class DxDeviceManager
	{
		static std::unique_ptr<DxDeviceManager> sm_spoInstance;
		DxDeviceManager() {}

	public:
		using SPtr = TSmartPointerAll<DxDeviceManager>;
	
	public:
		static DxDeviceManager* GetInstance() 
		{
			if (sm_spoInstance == nullptr) {
				sm_spoInstance = Create();
			}
			return sm_spoInstance.get();						
		};


	public:
		using UPtr = std::unique_ptr<DxDeviceManager>;	

	public:
		HRESULT InitDirect3d11();
		bool InitFont();

		void RenderText();

		

	private:
		static SPtr::Unique Create();

		/// <summary>
		/// Creates Direct 3d 11 device, and device context
		/// </summary>
		/// <returns></returns>

		//bool CreateDepthBuffer()
		//{

		//	//this->moScreenBuffer.initDepthStencilView(_poDevice, width(), height());







		//	//D3D11_DEPTH_STENCIL_DESC dsDesc;
		//	//// Depth initLog parameters
		//	//dsDesc.DepthEnable = true;
		//	//dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		//	//dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		//	//// Stencil initLog parameters
		//	//dsDesc.StencilEnable = false;
		//	//dsDesc.StencilReadMask = 0x00;
		//	//dsDesc.StencilWriteMask = 0x00;

		//	//// Stencil operations if pixel is front-facing
		//	//dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		//	//dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		//	//dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		//	//dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		//	//// Stencil operations if pixel is back-facing
		//	//dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		//	//dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		//	//dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		//	//dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		//	//// Create depth stencil state
		//	//ID3D11DepthStencilState* pDSState;
		//	//HRESULT hr = device()->CreateDepthStencilState(&dsDesc, &depthStencilStates.On);

		//	//assert(SUCCEEDED(hr));

		//	//dsDesc.DepthEnable = false;
		//	//hr = device()->CreateDepthStencilState(&dsDesc, &depthStencilStates.Off);

		//	//assert(SUCCEEDED(hr));


		//}


			/// <summary>
			/// creates swap chain linked native window
			/// </summary>
			/// <param name="hwnd">native win api window handle </param>
			/// <returns>new swap chain on sucess</returns>
			//DXSwapChain::sptrDXSwapChain createSwapChain(HWND hwnd);


		ID3D11Device* GetDevice() {
			return m_cpoDevice.Get();
		};
		ID3D11Device* GetDevice() const {
			return m_cpoDevice.Get();
		};

		ID3D11DeviceContext* GetDeviceContext() {
			return m_cpoDeviceContext.Get();
		};
		ID3D11DeviceContext* GetDeviceContext() const {
			return m_cpoDeviceContext.Get();
		};

		ID3D11Device* const* deviceAddress() const {
			return m_cpoDevice.GetAddressOf();
		};
		ID3D11Device* const* deviceAddress() {
			return m_cpoDevice.GetAddressOf();
		};

		ID3D11DeviceContext* const* GetAddressOfDeviceContext() const {
			return m_cpoDeviceContext.GetAddressOf();
		};
		ID3D11DeviceContext* const* GetAddressOfDeviceContext() {
			return m_cpoDeviceContext.GetAddressOf();
		};

		Microsoft::WRL::ComPtr<ID3D11Device>& GetReferenceToDevice() {
			return m_cpoDevice;
		}

		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetReferenceTiDviceContext() {
			return m_cpoDeviceContext;
		}

	protected:
		Microsoft::WRL::ComPtr<ID3D11Device> m_cpoDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_cpoDeviceContext;


	public:
		struct DepthStencilStates
		{
			Microsoft::WRL::ComPtr <ID3D11DepthStencilState> On;
			Microsoft::WRL::ComPtr <ID3D11DepthStencilState> Off;
		} depthStencilStates;

	private:




		struct FontEngine
		{
			std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
			std::unique_ptr<DirectX::SpriteFont> m_font;

			std::vector<std::wstring> m_stringsToDraw = { L"D3D\nIt Works\nNOW: Scenegrapdh" };
		} fontEngine;

	};
};




//DXSwapChain::sptrDXSwapChain createSwapChainForHwnd(HWND _hwnd, UINT width = 1024, UINT height = 1024)
//{
//
//	auto _poDevice = m_cpoDevice.Get();
//#if 1
//	DXSwapChain::sptrDXSwapChain po = std::make_unique< DXSwapChain>();
//	//DXSwapChain::sptrDXSwapChain po = DXSwapChain::create(_hwnd);
//
//	//----------------- get Factory2 interface ---------------
//	IDXGIDevice2* pDXGIDevice;
//	HRESULT hr = m_cpoDevice->QueryInterface(__uuidof(IDXGIDevice2), (void**)&pDXGIDevice);
//	assert(SUCCEEDED(hr));
//
//	IDXGIAdapter* pDXGIAdapter;
//	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDXGIAdapter);
//	assert(SUCCEEDED(hr));
//
//	IDXGIFactory2* pIDXGIFactory;
//	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&pIDXGIFactory);
//
//	//--------- create swap chain ------------------
//	RECT rcWindow;
//
//	// if no widht/height params supplied use window size
//	if (width != 0 && height != 0)
//	{
//		rcWindow.right = width;
//		rcWindow.bottom = height;
//	}
//	else
//	{
//		GetClientRect(_hwnd, &rcWindow);
//	}
//
//
//	po->m_oSawpChainInfo = DXGI_SWAP_CHAIN_DESC1{};
//	po->m_oSawpChainInfo.Width = rcWindow.right;
//	po->m_oSawpChainInfo.Height = rcWindow.bottom;
//	po->m_oSawpChainInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//	po->m_oSawpChainInfo.Stereo = FALSE;
//	po->m_oSawpChainInfo.SampleDesc.Quality = 0;
//	po->m_oSawpChainInfo.SampleDesc.Count = 1;
//	po->m_oSawpChainInfo.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	po->m_oSawpChainInfo.BufferCount = 3;
//	po->m_oSawpChainInfo.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH;
//	po->m_oSawpChainInfo.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
//	po->m_oSawpChainInfo.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
//	po->m_oSawpChainInfo.Flags = 0;
//
//	//IDXGISwapChain1* pSwapChain = nullptr; //= &m_oSwapChainData.m_pSwapChain1;
//	po->m_cpoSwapChain1 = nullptr; //= &m_oSwapChainData.m_pSwapChain1;
//	hr = pIDXGIFactory->CreateSwapChainForHwnd(_poDevice, _hwnd, &po->m_oSawpChainInfo, NULL, NULL, &po->m_cpoSwapChain1);
//	assert(SUCCEEDED(hr));
//	LogActionSuccess("Created Swap Chain.");
//
//	// Get swap chain's back buffer, create its renderQuad target view and set that view as renderQuad target
//
//	ID3D11Texture2D* oTempBackBuffer;
//	hr = po->m_cpoSwapChain1->GetBuffer(0, __uuidof(oTempBackBuffer), (void**)&po->m_oBackBuffer.m_cpoTexture);
//	assert(SUCCEEDED(hr));
//
//	LogActionSuccess("m_pSwapChain1->GetBuffer().");
//
//	hr = _poDevice->CreateRenderTargetView(po->m_oBackBuffer.getTexture(), nullptr, &po->m_oBackBuffer.getRenderTargetViewCPO());
//	assert(SUCCEEDED(hr));
//
//	LogActionSuccess("CreateRenderTargetView().");
//
//	//oTempBackBuffer->Release();
//	LogActionSuccess("m_oSwapChainData.m_pBackBuffer->Release().");
//
//	//m_c_poDeviceContext->OMSetRenderTargets(1, &po->m_oScreenBuffer.getRenderTargetViewCPO(), NULL);
//
//	///			LogActionSuccess("m_pDeviceContext->OMSetRenderTargets(1, &m_oSwapChainData.m_pRenderTargetView, NULL).");
//
//	return po;
//#endif
//	return nullptr;
//};
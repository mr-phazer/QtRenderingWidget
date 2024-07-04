#pragma once
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dwmapi.lib")
#pragma comment(lib, "d3d11.lib")


//#ifndef DIRECTINPUT_VERSION
//#define DIRECTINPUT_VERSION  0x0800 // to avoid compiler warnings about DirecIput version not set
//#endif // !DIRECTINPUT_VERSION

// Standard lib headers
#include <memory>

#include <d3d11.h>
//  WRL wrappers from COM Objects - smart pointers for Direct3d resources
#include <wrl/client.h>



#include "..\..\..\DirectXTK\Inc\CommonStates.h"
#include "DxDebugTextWriter.h"

namespace rldx
{
	/// <summary>
	/// singleton only 1 device per .exe
	/// </summary>
	class DxDeviceManager
	{


		DxDeviceManager() {};
		static std::unique_ptr<DxDeviceManager> sm_spoInstance;
	public:
		using PUnique = std::unique_ptr<DxDeviceManager>;

		struct DxD3dInterFaces { ID3D11Device* poDevice = nullptr; ID3D11DeviceContext* poDeviceContext = nullptr; };


		~DxDeviceManager() {
			auto DEBUG_1 = 1;
		}

	public:
		static void Init()
		{
			if (!sm_spoInstance)
			{
				sm_spoInstance = Create();
			}
		}

		static DxDeviceManager& GetInstance();

		static ID3D11DeviceContext* DeviceContext() {
			return GetInstance().GetDeviceContext();
		}

		static DxD3dInterFaces Devices() {
			return
			{
				GetInstance().GetDevice(),
				GetInstance().GetDeviceContext()
			};
		}

		static ID3D11Device* Device()
		{
			return GetInstance().GetDevice();
		}

		static 	DxDebugTextWriter* DebugTextWriter()
		{
			return GetInstance().GetDebugTextWriter();
		}

		DxDebugTextWriter* GetDebugTextWriter()
		{
			return m_upoDebugTextWriter.get();
		}

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

		ID3D11Device* const* GetAddressOfDevice() const {
			return m_cpoDevice.GetAddressOf();
		};
		ID3D11Device* const* GetAddressOfDevice() {
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

		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetReferenceToDviceContext() {
			return m_cpoDeviceContext;

		};

		DirectX::CommonStates& GetDxStates() {
			return *m_stateObjects;
		}

	private:
		static PUnique Create();
		HRESULT InitDirect3d11();
		bool InitFont();
		void RenderText();

	protected:
		Microsoft::WRL::ComPtr<ID3D11Device> m_cpoDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_cpoDeviceContext;
		std::unique_ptr<DirectX::CommonStates> m_stateObjects;

	public:
		struct DepthStencilStates
		{
			Microsoft::WRL::ComPtr <ID3D11DepthStencilState> On;
			Microsoft::WRL::ComPtr <ID3D11DepthStencilState> Off;
		} depthStencilStates;

	private:
		std::unique_ptr<DxDebugTextWriter> m_upoDebugTextWriter;
	};
};




// TODO: CLEAN up when everything works
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
#pragma once

// --- Direct3d 11 Headers
#include <d3d11.h>
#include <d3d11_1.h>
#include <D3Dcompiler.h>

//  WRL wrappers from COM Objects - smart pointers for Direct3d resources
#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>

// Standard lib headers
#include <assert.h>
#include <stdexcept>
#include <memory>
#include <functional>
#include <vector>


// qt headers
#include <QtWidgets/qwidget.h>

#include <DirectXMath.h>

// author headers:
#include "..\..\..\Logging\Logging.h"

// TODO: use new or old?
//#include "DxTextureView.h"
#include "DxTexture.h"

#include "IDrawable.h"


namespace Rldx
{
	class DxSwapChain
	{
	public:
		// TODO: is this relly needeed? How often do you need use the smart pointer type?
		using UPtr = std::unique_ptr<DxSwapChain>;
	public:
		DxSwapChain()
		{
			m_backgroundColor = Debug_GetRandomColor();
		}

		static DirectX::XMFLOAT4 Debug_GetRandomColor()
		{
			DirectX::XMFLOAT4 randomColor;
			const float divider = 100000;
			randomColor.x = ((float)(rand() % (int)divider)) / divider;
			randomColor.y = ((float)(rand() % (int)divider)) / divider;
			randomColor.z = ((float)(rand() % (int)divider)) / divider;
			randomColor.w = 1.0f;

			return randomColor;
		}

		static UPtr CreateForHWND(ID3D11Device* poDevice, HWND _hwnd, UINT width = 1024, UINT height = 1024)
		{
			auto poNew = std::make_unique<DxSwapChain>();

			//----------------- get Factory2 interface ---------------
			IDXGIDevice2* pDXGIDevice;
			HRESULT hr = poDevice->QueryInterface(__uuidof(IDXGIDevice2), (void**)&pDXGIDevice);
			assert(SUCCEEDED(hr));

			IDXGIAdapter* pDXGIAdapter;
			hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDXGIAdapter);
			assert(SUCCEEDED(hr));

			IDXGIFactory2* pIDXGIFactory;
			pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&pIDXGIFactory);

			//--------- create swap chain ------------------

			poNew->m_SwapChainDescription = DXGI_SWAP_CHAIN_DESC1{};
			poNew->m_SwapChainDescription.Width = width;
			poNew->m_SwapChainDescription.Height = height;
			poNew->m_SwapChainDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			poNew->m_SwapChainDescription.Stereo = FALSE;
			poNew->m_SwapChainDescription.SampleDesc.Quality = 0;
			poNew->m_SwapChainDescription.SampleDesc.Count = 1;
			poNew->m_SwapChainDescription.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
			poNew->m_SwapChainDescription.BufferCount = 3;
			poNew->m_SwapChainDescription.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH;
			poNew->m_SwapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			poNew->m_SwapChainDescription.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			poNew->m_SwapChainDescription.Flags = 0;

			IDXGISwapChain1** p = &poNew->m_cpoSwapChain1;
			hr = pIDXGIFactory->CreateSwapChainForHwnd(poDevice, _hwnd, &poNew->m_SwapChainDescription, NULL, NULL, &poNew->m_cpoSwapChain1);
			assert(SUCCEEDED(hr));
			logging::LogActionSuccess("Created Swap Chain.");

			// Get swap chain's back buffer, create its renderQuad target view and set that view as renderQuad target
			hr = poNew->m_cpoSwapChain1->GetBuffer(0, __uuidof(*poNew->m_BackBufferTexture.GetTexture()), (void**)&poNew->m_BackBufferTexture.GetTextureCPO());
			assert(SUCCEEDED(hr));

			logging::LogActionSuccess("m_pSwapChain1->GetBuffer().");

			//hr = m_cpoDevice->CreateRenderTargetView(po->m_oBackBuffer.getTexture(), nullptr, po->m_oBackBuffer.m_cpoRenderTargetView.ReleaseAndGetAddressOf());
			hr = poDevice->CreateRenderTargetView(poNew->m_BackBufferTexture.GetTexture(), nullptr, poNew->m_BackBufferTexture.GetRenderTargetViewCPO().ReleaseAndGetAddressOf());
			assert(SUCCEEDED(hr));

			logging::LogActionSuccess("CreateRenderTargetView().");

			//m_oSwapChainData.m_cpoBackBuffer->Release();
			logging::LogActionSuccess("m_oSwapChainData.m_pBackBuffer->Release().");

			// set render targer
			//deviceContext()->OMSetRenderTargets(1, po->m_oBackBuffer.getRenderTargetViewCPO(), NULL);

			//LogActionSuccess("m_pDeviceContext->OMSetRenderTargets(1, &m_oSwapChainData.m_pRenderTargetView, NULL).");

			return poNew;
		}

		void Reset(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height);
		void Present(ID3D11DeviceContext* poDXDeviceContext);
		DxTexture* GetBackBuffer() { return &m_BackBufferTexture; };
		void UpdateViewPort(ID3D11DeviceContext* _pDeviceContext, QWidget* _renderView);

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_cpoSwapChain1;
		DxTexture m_BackBufferTexture;
		DXGI_SWAP_CHAIN_DESC1 m_SwapChainDescription = { 0 };
		DirectX::XMFLOAT4 m_backgroundColor;

		// TODO: should be in camera?		
		D3D11_VIEWPORT m_viewPort;
	};
};

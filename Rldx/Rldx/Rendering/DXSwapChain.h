#pragma once

// --- Direct3d 11 Headers
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>

#include "DxTexture.h"

namespace rldx
{
	class DxSwapChain
	{
	public:
		// TODO: is this relly needeed? How often do you need use the smart pointer type?
		using Uptr = std::unique_ptr<DxSwapChain>;
	public:
		DxSwapChain()
		{
			m_backgroundColor = Debug_GetRandomColor();
		}

		void ReleaseBackBuffer(ID3D11DeviceContext* poPC)
		{
			HRESULT hr = S_OK;


			// Release the render target view based on the back buffer:
			//m_pRenderTarget.Reset();
			m_BackBufferTexture.GetComPtrRenderTargetView().Reset();


			// Release the back buffer itself:
			//pBackBuffer.Reset();
			m_BackBufferTexture.GetComPtrTexture().Reset();

			// The depth stencil will need to be resized, so release it (and view):
			//m_pDepthStencilView.Reset();
			m_BackBufferTexture.GetComPtrDepthStencilView().Reset();

			//m_pDepthStencil.Reset();
			m_BackBufferTexture.GetComPtrDepthStencil().Reset();


			// After releasing references to these resources, we need to call Flush() to 
			// ensure that Direct3D also releases any references it might still have to
			// the same resources - such as pipeline bindings.
			poPC->Flush();
		}

		void ConfigureBackBuffer(ID3D11Device* poDevice, ID3D11DeviceContext* poPC);;


		static DirectX::XMFLOAT4 Debug_GetRandomColor()
		{
			DirectX::XMFLOAT4 randomColor{ };
			const float divider = 100000;
			randomColor.x = ((float)(rand() % (int)divider)) / divider;
			randomColor.y = ((float)(rand() % (int)divider)) / divider;
			randomColor.z = ((float)(rand() % (int)divider)) / divider;
			randomColor.w = 1.0f;

			return randomColor;
		}

		static Uptr CreateForHWND(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, HWND hWindow, bool useSRGB, UINT width = 1024, UINT height = 1024);

		const rldx::DxSwapChain::Uptr& GetExtendedFactory();
		void Resize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height);
		void Present(ID3D11DeviceContext* poDXDeviceContext);
		DxTexture* GetBackBuffer() { return &m_BackBufferTexture; };
		void UpdateViewPort(ID3D11DeviceContext* _pDeviceContext, HWND hwnWindowHandle = NULL);

		// TODO: should be in camera?		
		D3D11_VIEWPORT m_viewPort;
		D3D11_TEXTURE2D_DESC m_textureDesc = { 0 };

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_cpoSwapChain1;
		DxTexture m_BackBufferTexture;
		DXGI_SWAP_CHAIN_DESC1 m_SwapChainDescription = { 0 };
		DirectX::XMFLOAT4 m_backgroundColor;
	};
};



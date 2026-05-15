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
		static Uptr CreateForHWND(ID3D11Device* poDevice, ID3D11DeviceContext* poDeviceContext, HWND hWindow, bool useSRGB, UINT width = 1024, UINT height = 1024);
				
		const rldx::DxSwapChain::Uptr& GetExtendedFactory();
		DxTexture* GetBackBuffer() { return m_BackBufferTexture.get(); };		
		
		void Resize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height);
		void Present(ID3D11DeviceContext* poDXDeviceContext);
		void UpdateViewPort(ID3D11DeviceContext* _pDeviceContext);

		void ReleaseBackBuffer(ID3D11DeviceContext* poPC);	

	private:
		void ConfigureBackBuffer(ID3D11Device* poDevice, ID3D11DeviceContext* poPC);;

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_cpoSwapChain1;
		std::unique_ptr<DxTexture> m_BackBufferTexture;
		DXGI_SWAP_CHAIN_DESC1 m_SwapChainDescription = { 0 };
		DirectX::XMFLOAT4 m_backgroundColor = Debug_GetRandomColor();
		
		// TODO: should be in camera?		
		D3D11_VIEWPORT m_viewPort;
		D3D11_TEXTURE2D_DESC m_textureDesc = { 0 };


		// TODO: make functional, or remove, or move to a debug class
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
	};
};


